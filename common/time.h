#ifndef _TIME_H_
#define _TIME_H_
//=====================================================================//
/*!	@file
	@brief	時間関数（ヘッダー）
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RX/blob/master/LICENSE
*/
//=====================================================================//
#include <stdint.h>
#include <sys/types.h>

// typedef uint32_t time_t;

// SUN:0 MON:1 THU:2 WED:3 THU:4 FRI:5 SAT:6

struct tm {
	uint8_t		tm_sec;     /* seconds after the minute - [0,59] */
	uint8_t		tm_min;     /* minutes after the hour - [0,59] */
	uint8_t		tm_hour;    /* hours since midnight - [0,23] */
	uint8_t		tm_mday;    /* day of the month - [1,31] */
	uint8_t		tm_mon;     /* months since January - [0,11] */
	uint16_t	tm_year;    /* years since 1900 */
	uint8_t		tm_wday;    /* days since Sunday - [0,6] */
	uint16_t	tm_yday;    /* days since January 1 - [0,365] */
	char		tm_isdst;   /* daylight savings time flag */
};

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------//
/*!
	@brief	西暦から、その年がうるう年かを判定
	@param[in]	year	西暦
	@return		うるう年なら「0」以外
*/
//-----------------------------------------------------------------//
char check_leap_year(short year);


//-----------------------------------------------------------------//
/*!
	@brief	西暦と、月から、その月の最大日数を得る。
	@param[in]	year	西暦
	@param[in]	mon		月（0 to 11)
	@return		「月」に対応する最大日数
*/
//-----------------------------------------------------------------//
char get_mday(short year, char mon);


//-----------------------------------------------------------------//
/*!
	@brief	西暦から、その年の総日数を得る。
	@param[in]	year	西暦
	@return		「年」に対応する総日数
*/
//-----------------------------------------------------------------//
short get_yday(short year);


//-----------------------------------------------------------------//
/*!
	@brief	西暦、月から、1970 年からの総日数を得る。
	@param[in]	year	西暦 1970 〜
	@param[in]	mon		月	[0..11]
	@param[in]	day		日	[1..31]
	@return		1970 年1月1日からの総日数
*/
//-----------------------------------------------------------------//
long get_total_day(short year, char mon, char day);


//-----------------------------------------------------------------//
/*!
	@brief	世界標準時間（グリニッジ）からのオフセットを取得
	@return	オフセット時間（秒）
*/
//-----------------------------------------------------------------//
time_t get_timezone_offset(void);


//-----------------------------------------------------------------//
/*!
	@brief	世界標準時間（グリニッジ）から、tm 構造体のメンバー
			を生成する。（スレッドセーフ用）
	@param[in]	tp
	@param[out]	res
	@return		グローバル tm 構造体のポインター
*/
//-----------------------------------------------------------------//
struct tm *gmtime_r(const time_t *, struct tm* res);


//-----------------------------------------------------------------//
/*!
	@brief	世界標準時間（グリニッジ）から、tm 構造体のメンバー
			を生成する。（互換性の為）
	@param[in]	tp
	@return		グローバル tm 構造体のポインター
*/
//-----------------------------------------------------------------//
struct tm *gmtime(const time_t *);


//-----------------------------------------------------------------//
/*!
	@brief	tm 構造体から、世界標準(グリニッジ)時間を得る@n
			※メンバー変数 tm_yday は再計算される。@n
			※メンバー変数 tm_wday は再計算される。@n
			※ tm_isdgt は無視される。
	@param[in]	tmp	tm 構造体のポインター@n
				※NULLの場合は、システムの構造体が使われる
	@return		GMT:1970年1月1日0時0分0秒(4:THU)からの経過時間（秒）
*/
//-----------------------------------------------------------------//
time_t mktime_gmt(const struct tm *tmp);


//-----------------------------------------------------------------//
/*!
	@brief	tm 構造体から（ローカル時間）、世界標準(グリニッジ)時間を得る@n
			※メンバー変数 tm_yday は再計算される。@n
			※メンバー変数 tm_wday は再計算される。@n
			※ tm_isdgt は無視される。
	@param[in]	tmp	tm 構造体のポインター@n
				※NULLの場合は、システムの構造体が使われる
	@return		GMT:1970年1月1日0時0分0秒(4:THU)からの経過時間（秒）
*/
//-----------------------------------------------------------------//
time_t mktime(const struct tm *);


//-----------------------------------------------------------------//
/*!
	@brief	このモジュールで利用している tm 構造体のポインターを得る。
	@return		tm 構造体のポインター
*/
//-----------------------------------------------------------------//
struct tm *get_tm(void);


//-----------------------------------------------------------------//
/*!
	@brief	格納されているデータを現地時間に変換
	@param[in]	timer	現地時間
	@return		tm 構造体のポインター
*/
//-----------------------------------------------------------------//
struct tm *localtime(const time_t *timer);


//-----------------------------------------------------------------//
/*!
	@brief	tm 構造体のコピー
	@param[in]	src	コピー元
	@param[out]	dst	コピー先
*/
//-----------------------------------------------------------------//
void copy_tm(const struct tm *src, struct tm *dst); 


//-----------------------------------------------------------------//
/*!
	@brief	「曜日」文字列を取得
	@param[in]	idx	インデックス
	@return 文字列（３文字）
*/
//-----------------------------------------------------------------//
const char* get_wday(uint8_t idx);


//-----------------------------------------------------------------//
/*!
	@brief	「月」文字列を取得
	@param[in]	idx	インデックス
	@return 文字列（３文字）
*/
//-----------------------------------------------------------------//
const char* get_mon(uint8_t idx);

#ifdef __cplusplus
}
#endif

#endif	// _TIME_H_
