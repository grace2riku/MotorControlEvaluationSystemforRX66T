#pragma once
//=====================================================================//
/*!	@file
	@brief	RX140 グループ・ペリフェラル
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2024 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RX/blob/master/LICENSE
*/
//=====================================================================//
#include <cstdint>

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  ペリフェラル種別
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	enum class peripheral : uint16_t {

		CAC,	///< クロック周波数精度測定回路

		DTC,	///< データトランスファコントローラ

		ELC,	///< イベントリンクコントローラ

		MTU0,	///< マルチファンクションタイマパルスユニット０
		MTU1,	///< マルチファンクションタイマパルスユニット１
		MTU2,	///< マルチファンクションタイマパルスユニット２
		MTU3,	///< マルチファンクションタイマパルスユニット３
		MTU4,	///< マルチファンクションタイマパルスユニット４
		MTU5,	///< マルチファンクションタイマパルスユニット５

		POE2,	///< ポートアウトプットイネーブル２

		TPU0,	///< 16 ビットタイマパルスユニット０
		TPU1,	///< 16 ビットタイマパルスユニット１
		TPU2,	///< 16 ビットタイマパルスユニット２
		TPU3,	///< 16 ビットタイマパルスユニット３
		TPU4,	///< 16 ビットタイマパルスユニット４
		TPU5,	///< 16 ビットタイマパルスユニット５

		TMR0,	///< 8 ビットタイマ０
		TMR1,	///< 8 ビットタイマ１
		TMR2,	///< 8 ビットタイマ２
		TMR3,	///< 8 ビットタイマ３

		CMT0,	///< コンペアマッチタイマー０
		CMT1,	///< コンペアマッチタイマー１

		RTC,	///< リアルタイムクロック

		LPT,	///< ローパワータイマ

		IWDT,	///< 独立ウォッチドッグタイマ

		SCI1,	///< シリアルコミュニケーションインタフェース１
		SCI5,	///< シリアルコミュニケーションインタフェース５
		SCI6,	///< シリアルコミュニケーションインタフェース６
		SCI8,	///< シリアルコミュニケーションインタフェース８
		SCI9,	///< シリアルコミュニケーションインタフェース９
		SCI11,	///< シリアルコミュニケーションインタフェース１１
		SCI12,	///< シリアルコミュニケーションインタフェース１２

		RIIC0,	///< I 2 C バスインタフェース０

		RSCAN,	///< CAN モジュール

		RSPI0,	///< シリアルペリフェラルインタフェース０

		CRC,	///< CRC 演算器（CRC）

		CTSU,	///< 静電容量式タッチセンサ

		S12AD,	///< 12 ビット A/D コンバータ０

		DA,		///< 12 ビット D/A コンバータ

		CMPB,	///< コンパレータＢ

		DOC,	///< データ演算回路（DOC）
	};
}