#pragma once
//=====================================================================//
/*!	@file
	@brief	RX66T グループ・ペリフェラル
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2018 Kunihito Hiramatsu @n
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

		CAC,		///< クロック周波数精度測定回路

		DTC,		///< データトランスファコントローラ

		ELC,		///< イベントリンクコントローラ (ELC)

		DMAC0,		///< DMA コントローラ・チャネル０
		DMAC1,		///< DMA コントローラ・チャネル１
		DMAC2,		///< DMA コントローラ・チャネル２
		DMAC3,		///< DMA コントローラ・チャネル３
		DMAC4,		///< DMA コントローラ・チャネル４
		DMAC5,		///< DMA コントローラ・チャネル５
		DMAC6,		///< DMA コントローラ・チャネル６
		DMAC7,		///< DMA コントローラ・チャネル７

		MTU0,		///< マルチファンクションタイマパルスユニット０
		MTU1,		///< マルチファンクションタイマパルスユニット１
		MTU2,		///< マルチファンクションタイマパルスユニット２
		MTU3,		///< マルチファンクションタイマパルスユニット３
		MTU4,		///< マルチファンクションタイマパルスユニット４
		MTU5,		///< マルチファンクションタイマパルスユニット５
		MTU6,		///< マルチファンクションタイマパルスユニット６
		MTU7,		///< マルチファンクションタイマパルスユニット７
		MTU9,		///< マルチファンクションタイマパルスユニット９

		POE3,		///< ポートアウトプットイネーブル

		GPTW0,		///< GPTW0  汎用 PWM タイマ
		GPTW1,		///< GPTW1  汎用 PWM タイマ
		GPTW2,		///< GPTW2  汎用 PWM タイマ
		GPTW3,		///< GPTW3  汎用 PWM タイマ
		GPTW4,		///< GPTW4  汎用 PWM タイマ
		GPTW5,		///< GPTW5  汎用 PWM タイマ
		GPTW6,		///< GPTW6  汎用 PWM タイマ
		GPTW7,		///< GPTW7  汎用 PWM タイマ
		GPTW8,		///< GPTW8  汎用 PWM タイマ
		GPTW9,		///< GPTW9  汎用 PWM タイマ

		HRPWM,		///< 高分解能 PWM 波形生成回路

		POEG,		///< GPTW 用ポートアウトプットイネーブル (POEG)

		TMR0,		///< 8 ビットタイマ０
		TMR1,		///< 8 ビットタイマ１
		TMR2,		///< 8 ビットタイマ２
		TMR3,		///< 8 ビットタイマ３
		TMR4,		///< 8 ビットタイマ４
		TMR5,		///< 8 ビットタイマ５
		TMR6,		///< 8 ビットタイマ６
		TMR7,		///< 8 ビットタイマ７

		CMT0,		///< コンペアマッチタイマ０（CMT）
		CMT1,		///< コンペアマッチタイマ１（CMT）
		CMT2,		///< コンペアマッチタイマ２（CMT）
		CMT3,		///< コンペアマッチタイマ３（CMT）

		WDTA,		///< ウォッチドッグタイマ
		IWDT,		///< 独立ウォッチドッグタイマ

		USB0,		///< USB2.0FSホスト/ファンクションモジュール（USBb）

		SCI1C,		///< SCI1/CLK  シリアルコミュニケーションインタフェース
		SCI1,		///< SCI1      シリアルコミュニケーションインタフェース
		SCI5C,		///< SCI5/CLK  シリアルコミュニケーションインタフェース
		SCI5,		///< SCI5      シリアルコミュニケーションインタフェース
		SCI6C,		///< SCI6/CLK  シリアルコミュニケーションインタフェース
		SCI6,		///< SCI6      シリアルコミュニケーションインタフェース
		SCI8C,		///< SCI8/CLK  シリアルコミュニケーションインタフェース
		SCI8,		///< SCI8      シリアルコミュニケーションインタフェース
		SCI9C,		///< SCI9/CLK  シリアルコミュニケーションインタフェース
		SCI9,		///< SCI9      シリアルコミュニケーションインタフェース
		SCI11C,		///< SCI11/CLK シリアルコミュニケーションインタフェース
		SCI11,		///< SCI11     シリアルコミュニケーションインタフェース
		SCI12C,		///< SCI12/CLK シリアルコミュニケーションインタフェース
		SCI12,		///< SCI12     シリアルコミュニケーションインタフェース

		RIIC0,		///< I 2 C バスインタフェース０（RIICa）

		CAN0,		///< CAN インタフェース（CAN0）

		RSPI0,		///< シリアルペリフェラルインタフェース（RSPI）

		CRC,		///< CRC 演算器 (CRCA)

		TSIP,		///< Trusted Secure IP (TSIP-Lite)

		S12AD,		///< 12 ビット A/D コンバータ（S12ADC）
		S12AD1,		///< 12 ビット A/D コンバータ（S12ADC）
		S12AD2,		///< 12 ビット A/D コンバータ（S12ADC）

		R12DA,		///< 12 ビット D/A コンバータ（R12DA）

		TEMPS,		///< 温度センサ（TEMPS）

		CMPC0,		///< コンパレーター０
		CMPC1,		///< コンパレーター１
		CMPC2,		///< コンパレーター２
		CMPC3,		///< コンパレーター３
		CMPC4,		///< コンパレーター４
		CMPC5,		///< コンパレーター５

		DOC,		///< データ演算回路 (DOC)

		ECCRAM,		///< ECC RAM (32K)

	};
}
