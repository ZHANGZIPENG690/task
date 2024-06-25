// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
#include "core/fset_lib.h"
#include <math.h>
// #include "time.h"
// #include "hot_so_fast.h"
// #include "tool.h"

// extern time_manage _time;

#define M_PI 3.14159265358979323846

static double RAD = 180.0 * 3600 / M_PI;
static double richu;
static double midDayTime;
static double dawnTime;
static double jd;
static double wd;
static float jd_degrees;
static float wd_degrees;

// bool update_suninfo = false;
/**
 * @brief 儒略日的计算
 *
 * @param y 年
 * @param M 月
 * @param d 日
 * @return double
 */
static double timeToDouble(int y, int M, double d)
{
    //        double A=0;
    double B = 0;
    double jd = 0;

    // 设Y为给定年份，M为月份，D为该月日期（可以带小数）。
    // 若M > 2，Y和M不变，若 M =1或2，以Y–1代Y，以M+12代M，换句话说，如果日期在1月或2月，则被看作是在前一年的13月或14月。
    // 对格里高利历有 ：A = INT（Y/100）   B = 2 - A + INT(A/4)
    // 对儒略历，取 B = 0
    //  JD = INT(365.25(Y+4716))+INT(30.6001(M+1))+D+B-1524.5 （7.1）
    B = -13;
    jd = floor(365.25 * (y + 4716)) + floor(30.60001 * (M + 1)) + B + d - 1524.5;
    return jd;
}

static uint64_t double_to_timesp(double time, uint32_t *suninfo)
{
    uint64_t result = 0;
    double t;
    int h = 0, m = 0, s = 0;
    t = time + 0.5;
    t = (t - (int)t) * 24;
    h = (int)t;
    t = (t - h) * 60;
    m = (int)t;
    t = (t - m) * 60;
    s = (int)t;
    // snprintf(str, 25, "%02d-%02d-%02d", (uint32_t)abs(h), (uint32_t)abs(m), 0);
    *suninfo = (uint32_t)abs(h) * 60 * 60 + (uint32_t)abs(m) * 60;
    // *sunset = (uint32_t)abs(h) * 60 * 60 + (uint32_t)abs(m) * 60;
    h = 0,
    m = 0, s = 0;
    return result;
}

/****************************
 * @param t 儒略世纪数
 *
 * @return 太阳黄经
 *****************************/
static double sunHJ(double t)
{
    double j;
    t = t + (32.0 * (t + 1.8) * (t + 1.8) - 20) / 86400.0 / 36525.0;
    // 儒略世纪年数,力学时
    j = 48950621.66 + 6283319653.318 * t + 53 * t * t - 994 + 334166 * cos(4.669257 + 628.307585 * t) + 3489 * cos(4.6261 + 1256.61517 * t) + 2060.6 * cos(2.67823 + 628.307585 * t) * t;
    return (j / 10000000);
}

static double mod(double num1, double num2)
{
    num2 = fabs(num2);
    // 只是取决于Num1的符号
    return num1 >= 0 ? (num1 - (floor(num1 / num2)) * num2) : ((floor(fabs(num1) / num2)) * num2 - fabs(num1));
}
/********************************
 * 保证角度∈(-π,π)
 *
 * @param ag
 * @return ag
 ***********************************/
static double degree(double ag)
{
    ag = mod(ag, 2 * M_PI);
    if (ag <= -M_PI)
    {
        ag = ag + 2 * M_PI;
    }
    else if (ag > M_PI)
    {
        ag = ag - 2 * M_PI;
    }
    return ag;
}

/**
 * @brief 日出
 *
 * @param date 儒略日平午
 * @param lo 经度
 * @param la 纬度
 * @param tz 时区
 * @return double
 */
double sunRiseTime(double date, double lo, double la, double tz)
{
    double t, j, sinJ, cosJ, gst, E, a, D, cosH0, cosH1, H0, H1, H;
    date = date - tz;
    // 太阳黄经以及它的正余弦值
    t = date / 36525;
    j = sunHJ(t);
    // 太阳黄经以及它的正余弦值
    sinJ = sin(j);
    cosJ = cos(j);
    // 其中2*M_PI*(0.7790572732640 + 1.00273781191135448*jd)恒星时（子午圈位置）
    gst = 2 * M_PI * (0.779057273264 + 1.00273781191135 * date) + (0.014506 + 4612.15739966 * t + 1.39667721 * t * t) / RAD;
    E = (84381.406 - 46.836769 * t) / RAD;                                  // 黄赤交角
    a = atan2(sinJ * cos(E), cosJ);                                         // '太阳赤经
    D = asin(sin(E) * sinJ);                                                // 太阳赤纬
    cosH0 = (sin(-50 * 60 / RAD) - sin(la) * sin(D)) / (cos(la) * cos(D));  // 日出的时角计算，地平线下50分
    cosH1 = (sin(-6 * 3600 / RAD) - sin(la) * sin(D)) / (cos(la) * cos(D)); // 天亮的时角计算，地平线下6度，若为航海请改为地平线下12度
    // 严格应当区分极昼极夜，本程序不算
    if (cosH0 >= 1 || cosH0 <= -1)
    {
        return -0.5; // 极昼
    }
    H0 = -acos(cosH0); // 升点时角（日出）若去掉负号 就是降点时角，也可以利用中天和升点计算
    H1 = -acos(cosH1);
    H = gst - lo - a;                                 // 太阳时角
    midDayTime = date - degree(H) / M_PI / 2 + tz;    // 中天时间
    dawnTime = date - degree(H - H1) / M_PI / 2 + tz; // 天亮时间
    return date - degree(H - H0) / M_PI / 2 + tz;     // 日出时间，函数返回值
}

void m_callable_suninfo_get_sunset_and_rise(float lng, float lat, int tz, int year, int month, int day, uint32_t *sunset, uint32_t *sunrise)
{
    // char timestr[20];
    DEBUG_TEST(DB_W,"lng = %f ,lat =  %f ,tz =  %d ,year =  %d ,month =  %d ,day =  %d ", lng, lat, tz, year, month, day);
    if (lng == 0 || lat == 0)
    {
        *sunset = 63300;
        *sunrise = 23880;
        return;
    }
    jd_degrees = lng;
    wd_degrees = lat;
    // step 1
    jd = -(jd_degrees) / 180 * M_PI;
    wd = (wd_degrees) / 180 * M_PI;

    richu = timeToDouble(year, month, day) - 2451544.5;

    for (int y = 0; y < 10; y++)
    {
        richu = sunRiseTime(richu, jd, wd, tz / 24.0); // 逐步逼近法算10次
    }
    // DEBUG_TEST(DB_I,"%d年%d月%d日\t", year, month, day);

    double_to_timesp(richu, sunrise);
    double_to_timesp( midDayTime + midDayTime - richu, sunset);
    // printf("_time.todaysunrise : %s\r\n", _time.toTime(_time.todaysunrise));
    // printf("_time.todaysunset : %s\r\n", _time.toTime(_time.todaysunset));
    // printf(" srise %s", _time.today_sunrise);
    // printf(" sset%s\r\n", _time.today_sunset);

    // printf("\r\n\t\t--------%d---%d----%02d------------\r\n\r\n", year, month + 1, 0);
}