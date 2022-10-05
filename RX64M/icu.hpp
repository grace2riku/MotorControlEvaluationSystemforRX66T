#pragma once
//=============================================================================//
/*!	@file
	@brief	RX64M/RX71M グループ・ICUA 定義 @n
			・RIIC の割り込み名は、SCI の割り込み名と区別出来ない為、変更している。@n
			・Ex: RIIC0 (RXI0  --->  RIIC_RXI0) 
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2016, 2022 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RX/blob/master/LICENSE
*/
//=============================================================================//
#include "RX600/icu_utils.hpp"
#include "RX600/icu_base.hpp"

namespace device {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief  RX64M/RX71M 割り込みコントローラ・テンプレート・クラス（ICUA）
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class _>
	struct icu_t : public icu_base {

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  通常割り込みベクター型
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class VECTOR : uint8_t {

			NONE         = 0,   ///< ベクター０

			BUSERR       = 16,  ///< BSC

			RAMERR       = 18,  ///< RAM

			FIFERR       = 21,  ///< FCU

			FRDYI        = 23,  ///< FCU

			SWINT2       = 26,  ///< ICU
			SWINT        = 27,  ///< ICU

			CMI0         = 28,  ///< CMT0
			CMI1         = 29,  ///< CMT1
			CMWI0        = 30,  ///< CMTW0
			CMWI1        = 31,  ///< CMTW1

			D0FIFO2      = 32,  ///< USBA
			D1FIFO2      = 33,
			D0FIFO0      = 34,  ///< USB0
			D1FIFO0      = 35,

			SPRI0        = 38,  ///< RSPI0
			SPTI0        = 39,
#if defined(SIG_RX71M)
			SPRI1        = 40,  ///< RSPI1
			SPTI1        = 41,
#endif
			SPRI         = 42,  ///< QSPI
			SPTI         = 43,

			SBFAI        = 44,  ///< SDHI

			MBFAI        = 45,  ///< MMCIF
			SSITXI0      = 46,  ///< SSI0 シリアルサウンドインタフェース
			SSIRXI0      = 47,
			SSIRTI1      = 48,  ///< SSI1 シリアルサウンドインタフェース

			IDEI         = 50,  ///< SRC
			ODFI         = 51,
			ICRXI0		 = 52,  ///< RIIC0
			ICTXI0		 = 53,
			ICRXI2		 = 54,  ///< RIIC2
			ICTXI2		 = 55,

			RXI0         = 58,  ///< SCI0
			TXI0         = 59,
			RXI1         = 60,  ///< SCI1
			TXI1         = 61,
			RXI2         = 62,  ///< SCI2
			TXI2         = 63, 

			IRQ0         = 64,  ///< ICU
			IRQ1         = 65,
			IRQ2         = 66,
			IRQ3         = 67,
			IRQ4         = 68,
			IRQ5         = 69,
			IRQ6         = 70,
			IRQ7         = 71,
			IRQ8         = 72,
			IRQ9         = 73,
			IRQ10        = 74,
			IRQ11        = 75,
			IRQ12        = 76,
			IRQ13        = 77,
			IRQ14        = 78,
			IRQ15        = 79,

			RXI3         = 80,  ///< SCI3
			TXI3         = 81,
			RXI4         = 82,  ///< SCI4
			TXI4         = 83,
			RXI5         = 84,  ///< SCI5
			TXI5         = 85,
			RXI6         = 86,  ///< SCI6
			TXI6         = 87,

			LVD1         = 88,  ///< LVD1
			LVD2         = 89,  ///< LVD2

			USBR0        = 90,  ///< USB0

			ALM          = 92,  ///< RTC
			PRD          = 93,
			USBAR        = 94,  ///< USBA

			IWUNI        = 95,  ///< IWDT
			WUNI         = 96,  ///< WDT
			PCDFI        = 97,  ///< PDC

			RXI7         = 98,  ///< SCI7
			TXI7         = 99,
			RXIF8        = 100, ///< SCIFA8
			TXIF8        = 101,
			RXIF9        = 102, ///< SCIFA9
			TXIF9        = 103,
			RXIF10       = 104, ///< SCIFA10
			TXIF10       = 105,

			GROUPBE0     = 106,  ///< ICU

			GROUPBL0	 = 110,  ///< ICU
			GROUPBL1	 = 111,  ///< ICU
			GROUPAL0	 = 112,  ///< ICU
			GROUPAL1	 = 113,  ///< ICU
			RXIF11  	 = 114,  ///< SCIFA11
			TXIF11   	 = 115,
			RXI12   	 = 116,  ///< SCI12
			TXI12   	 = 117,

			DMAC0I  	 = 120,  ///< DMAC
			DMAC1I  	 = 121,
			DMAC2I  	 = 122,
			DMAC3I  	 = 123,
			DMAC74I 	 = 124,

			OSTDI    	 = 125,  ///< OST

			EXDMAC0I 	 = 126,  ///< EXDMAC 0
			EXDMAC1I 	 = 127,	 ///< EXDMAC 1

			INTB128  	 = 128,
			INTB129,
			INTB130,
			INTB131,
			INTB132,
			INTB133,
			INTB134,
			INTB135,
			INTB136,
			INTB137,
			INTB138,
			INTB139,
			INTB140,
			INTB141,
			INTB142,
			INTB143,
			INTB144,
			INTB145,
			INTB146,
			INTB147,
			INTB148,
			INTB149,
			INTB150,
			INTB151,
			INTB152,
			INTB153,
			INTB154,
			INTB155,
			INTB156,
			INTB157,
			INTB158,
			INTB159,
			INTB160,
			INTB161,
			INTB162,
			INTB163,
			INTB164,
			INTB165,
			INTB166,
			INTB167,
			INTB168,
			INTB169,
			INTB170,
			INTB171,
			INTB172,
			INTB173,
			INTB174,
			INTB175,
			INTB176,
			INTB177,
			INTB178,
			INTB179,
			INTB180,
			INTB181,
			INTB182,
			INTB183,
			INTB184,
			INTB185,
			INTB186,
			INTB187,
			INTB188,
			INTB189,
			INTB190,
			INTB191,
			INTB192,
			INTB193,
			INTB194,
			INTB195,
			INTB196,
			INTB197,
			INTB198,
			INTB199,
			INTB200,
			INTB201,
			INTB202,
			INTB203,
			INTB204,
			INTB205,
			INTB206,
			INTB207,

			INTA208  = 208,
			INTA209,
			INTA210,
			INTA211,
			INTA212,
			INTA213,
			INTA214,
			INTA215,
			INTA216,
			INTA217,
			INTA218,
			INTA219,
			INTA220,
			INTA221,
			INTA222,
			INTA223,
			INTA224,
			INTA225,
			INTA226,
			INTA227,
			INTA228,
			INTA229,
			INTA230,
			INTA231,
			INTA232,
			INTA233,
			INTA234,
			INTA235,
			INTA236,
			INTA237,
			INTA238,
			INTA239,
			INTA240,
			INTA241,
			INTA242,
			INTA243,
			INTA244,
			INTA245,
			INTA246,
			INTA247,
			INTA248,
			INTA249,
			INTA250,
			INTA251,
			INTA252,
			INTA253,
			INTA254,
			INTA255,
		};


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  選択型割り込みＢ要因・ベクター型
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class VECTOR_SELB : uint8_t {
			NONE = 0,	///< なし

			CMI2 = 1,		///< CMT2 (コンペアマッチ)
			CMI3 = 2,		///< CMT3 (コンペアマッチ)

			CMIA0 = 3,		///< TMR0 (TCORA のコンペアマッチ)
			CMIB0,			///< TMR0 (TCORB のコンペアマッチ)
			OVI0,			///< TMR0 (TCNT のオーバフロー)
			CMIA1,			///< TMR1 (TCORA のコンペアマッチ)
			CMIB1,			///< TMR1 (TCORB のコンペアマッチ)
			OVI1,			///< TMR1 (TCNT のオーバフロー)
			CMIA2,			///< TMR2 (TCORA のコンペアマッチ)
			CMIB2,			///< TMR2 (TCORB のコンペアマッチ)
			OVI2,			///< TMR2 (TCNT のオーバフロー)
			CMIA3,			///< TMR3 (TCORA のコンペアマッチ)
			CMIB3,			///< TMR3 (TCORB のコンペアマッチ)
			OVI3,			///< TMR3 (TCNT のオーバフロー)
			TGI0A,			///< TPU0 (TGRA のインプットキャプチャ / コンペアマッチ)
			TGI0B,			///< TPU0 (TGRB のインプットキャプチャ / コンペアマッチ)
			TGI0C,			///< TPU0 (TGRC のインプットキャプチャ / コンペアマッチ)
			TGI0D,			///< TPU0 (TGRD のインプットキャプチャ / コンペアマッチ)
			TGI0V,			///< TPU0 (TCNT のオーバフロー)
			TGI1A,			///< TPU1 (TGRA のインプットキャプチャ / コンペアマッチ)
			TGI1B,			///< TPU1 (TGRB のインプットキャプチャ / コンペアマッチ)
			TGI1V,			///< TPU1 (TCNT のオーバフロー)
			TGI1U,			///< TPU1 (TCNT のアンダーフロー)
			TGI2A,			///< TPU2 (TGRA のインプットキャプチャ / コンペアマッチ)
			TGI2B,			///< TPU2 (TGRB のインプットキャプチャ / コンペアマッチ)
			TGI2V,			///< TPU2 (TCNT のオーバフロー)
			TGI2U,			///< TPU2 (TCNT のアンダーフロー)
			TGI3A,			///< TPU3 (TGRA のインプットキャプチャ / コンペアマッチ)
			TGI3B,			///< TPU3 (TGRB のインプットキャプチャ / コンペアマッチ)
			TGI3C,			///< TPU3 (TGRC のインプットキャプチャ / コンペアマッチ)
			TGI3D,			///< TPU3 (TGRD のインプットキャプチャ / コンペアマッチ)
			TGI3V,			///< TPU3 (TCNT のオーバフロー)
			TGI4A,			///< TPU4 (TGRA のインプットキャプチャ / コンペアマッチ)
			TGI4B,			///< TPU4 (TGRB のインプットキャプチャ / コンペアマッチ)
			TGI4V,			///< TPU4 (TCNT のオーバフロー)
			TGI4U,			///< TPU4 (TCNT のアンダーフロー)
			TGI5A,			///< TPU5 (TGRA のインプットキャプチャ / コンペアマッチ)
			TGI5B,			///< TPU5 (TGRB のインプットキャプチャ / コンペアマッチ)
			TGI5V,			///< TPU5 (TCNT のオーバフロー)
			TGI5U,			///< TPU5 (TCNT のアンダーフロー)
			IC0I0,			///< CMTW0 (CMWICR0 レジスタのインプットキャプチャ入力)
			IC1I0,			///< CMTW0 (CMWICR1 レジスタのインプットキャプチャ入力)
			OC0I0,			///< CMTW0 (CMWOCR0 レジスタのアウトプットコンペア出力)
			OC1I0,			///< CMTW0 (CMWOCR1 レジスタのアウトプットコンペア出力)
			IC0I1,			///< CMTW1 (CMWICR0 レジスタのインプットキャプチャ入力)
			IC1I1,			///< CMTW1 (CMWICR1 レジスタのインプットキャプチャ入力)
			OC0I1,			///< CMTW1 (CMWOCR0 レジスタのアウトプットコンペア出力)
			OC1I1,			///< CMTW1 (CMWOCR1 レジスタのアウトプットコンペア出力)
			CUP,			///< RTC (桁上げ割り込み)
			RXF0 = 50,		///< CAN0（受信 FIFO 割り込み）
			TXF0 = 51,		///< CAN0（送信 FIFO 割り込み）
			RXM0 = 52,		///< CAN0（メールボックス０～３１メッセージ受信完了）
			TXM0 = 53,		///< CAN0（メールボックス０～３１メッセージ送信完了）
			RXF1 = 54,		///< CAN1（受信 FIFO 割り込み）
			TXF1 = 55,		///< CAN1（送信 FIFO 割り込み）
			RXM1 = 56,		///< CAN1（メールボックス０～３１メッセージ受信完了）
			TXM1 = 57,		///< CAN1（メールボックス０～３１メッセージ送信完了）
			RXF2 = 58,		///< CAN2（受信 FIFO 割り込み）
			TXF2 = 59,		///< CAN2（送信 FIFO 割り込み）
			RXM2 = 60,		///< CAN2（メールボックス０～３１メッセージ受信完了）
			TXM2 = 61,		///< CAN2（メールボックス０～３１メッセージ送信完了）

			USBI0 = 62,		///< USB0（１５要因のステータス割り込み） 
		};


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  選択型割り込みＡ要因・ベクター型
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class VECTOR_SELA : uint8_t {
			NONE = 0,		///< なし

			TGIA0 = 1,		///< MTU0（TGRAのインプットキャプチャ /コンペアマッチ）
			TGIB0,			///< MTU0（TGRBのインプットキャプチャ /コンペアマッチ）
			TGIC0,			///< MTU0（TGRCのインプットキャプチャ /コンペアマッチ）
			TGID0,			///< MTU0（TGRDのインプットキャプチャ /コンペアマッチ）
			TCIV0,			///< MTU0（TCNTのオーバフロー）
			TGIE0,			///< MTU0（TGREのインプットキャプチャ /コンペアマッチ）
			TGIF0,			///< MTU0（TGRFのインプットキャプチャ /コンペアマッチ）

			TGIA1 = 8,		///< MTU1（TGRAのインプットキャプチャ /コンペアマッチ）
			TGIB1,			///< MTU1（TGRBのインプットキャプチャ /コンペアマッチ）
			TCIV1,			///< MTU1（TCNTのオーバフロー）
			TCIU1,			///< MTU1（TCNTのアンダフロー）

			TGIA2 = 12,		///< MTU2（TGRAのインプットキャプチャ /コンペアマッチ）
			TGIB2,			///< MTU2（TGRBのインプットキャプチャ /コンペアマッチ）
			TCIV2,			///< MTU2（TCNTのオーバフロー）
			TCIU2,			///< MTU2（TCNTのアンダフロー）

			TGIA3 = 16,		///< MTU3（TGRAのインプットキャプチャ /コンペアマッチ）
			TGIB3,			///< MTU3（TGRBのインプットキャプチャ /コンペアマッチ）
			TGIC3,			///< MTU3（TGRCのインプットキャプチャ /コンペアマッチ）
			TGID3,			///< MTU3（TGRDのインプットキャプチャ /コンペアマッチ）
			TCIV3,			///< MTU3（TCNTのオーバフロー）

			TGIA4 = 21,		///< MTU4（TGRAのインプットキャプチャ /コンペアマッチ）
			TGIB4,			///< MTU4（TGRBのインプットキャプチャ /コンペアマッチ）
			TGIC4,			///< MTU4（TGRCのインプットキャプチャ /コンペアマッチ）
			TGID4,			///< MTU4（TGRDのインプットキャプチャ /コンペアマッチ）
			TCIV4,			///< MTU4（TCNTのオーバフロー /アンダフロー（相補PWMモード時のみ））

			TGIU5 = 27,		///< MTU5（TGRUのインプットキャプチャ /コンペアマッチ）
			TGIV5,			///< MTU5（TGRVのインプットキャプチャ /コンペアマッチ）
			TGIW5,			///< MTU5（TGRWのインプットキャプチャ /コンペアマッチ）

			TGIA6 = 30,		///< MTU6（TGRAのインプットキャプチャ /コンペアマッチ）
			TGIB6,			///< MTU6（TGRBのインプットキャプチャ /コンペアマッチ）
			TGIC6,			///< MTU6（TGRCのインプットキャプチャ /コンペアマッチ）
			TGID6,			///< MTU6（TGRDのインプットキャプチャ /コンペアマッチ）
			TCIV6,			///< MTU6（TCNTのオーバフロー）

			TGIA7 = 35,		///< MTU7（TGRAのインプットキャプチャ /コンペアマッチ）
			TGIB7,			///< MTU7（TGRBのインプットキャプチャ /コンペアマッチ）
			TGIC7,			///< MTU7（TGRCのインプットキャプチャ /コンペアマッチ）
			TGID7,			///< MTU7（TGRDのインプットキャプチャ /コンペアマッチ）
			TCIV7,			///< MTU7（TCNTのオーバフロー /アンダフロー（相補PWMモード時のみ））

			TGIA8 = 41,		///< MTU8（TGRAのインプットキャプチャ /コンペアマッチ）
			TGIB8,			///< MTU8（TGRBのインプットキャプチャ /コンペアマッチ）
			TGIC8,			///< MTU8（TGRCのインプットキャプチャ /コンペアマッチ）
			TGID8,			///< MTU8（TGRDのインプットキャプチャ /コンペアマッチ）
			TCIV8,			///< MTU8（TCNTのオーバフロー）
		};


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  GROUPBE0・ベクター型
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class VECTOR_BE0 : uint8_t {
			ERS0,			///< CAN0 / ERS0
			ERS1,			///< CAN1 / ERS1
			ERS2			///< CAN2 / ERS2
		};


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  GROUPBL0・ベクター型
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class VECTOR_BL0 : uint8_t {
			TEI0,			///< SCI0 / TEI0（送信完了）
			ERI0,			///< SCI0 / ERI0（受信エラー）
			TEI1,			///< SCI1 / TEI1（送信完了）
			ERI1,			///< SCI1 / ERI1（受信エラー）
			TEI2,			///< SCI2 / TEI2（送信完了）
			ERI2,			///< SCI2 / ERI2（受信エラー）
			TEI3,			///< SCI3 / TEI3（送信完了）
			ERI3,			///< SCI3 / ERI3（受信エラー）
			TEI4,			///< SCI4 / TEI4（送信完了）
			ERI4,			///< SCI4 / ERI4（受信エラー）
			TEI5,			///< SCI5 / TEI5（送信完了）
			ERI5,			///< SCI5 / ERI5（受信エラー）
			TEI6,			///< SCI6 / TEI6（送信完了）
			ERI6,			///< SCI6 / ERI6（受信エラー）
			TEI7,			///< SCI7 / TEI7（送信完了）
			ERI7,			///< SCI7 / ERI7（受信エラー）
			TEI12,			///< SCI12 / TEI12（送信完了）
			ERI12,			///< SCI12 / ERI12（受信エラー）
			SCIX0,			///< SCI12 / SCIX0
			SCIX1,			///< SCI12 / SCIX1
			SCIX2,			///< SCI12 / SCIX2
			SCIX3,			///< SCI12 / SCIX3

			QSPSSLI = 24,	///< QSPI / QSPSSLI

			FERRI = 26,		///< CAC / FERRI
			MENDI,			///< CAC / MENDI
			OVFI,			///< CAC / OVFI
			DOPCI,			///< DOC / DOPCI
			PCFEI,			///< PDC / PCFEI
			PCERI			///< PDC / PCERI
		};


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  GROUPBL1・ベクター型
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class VECTOR_BL1 : uint8_t {
			OVFI = 0,		///< SRC / OVFI
			UDFI,			///< SRC / UDFI
			CEFI,			///< SRC / CEFI
			CDETI = 3,		///< SDHI / CDETI
			CACI,			///< SDHI / CACI
			SDACI,			///< SDHI / SDACI
			CDETIO,			///< MMCIF / CDETIO
			ERRIO,			///< MMCIF / ERRIO
			ACCIO,			///< MMCIF / ACCIO
			OEI1,			///< POE3 / OEI1
			OEI2,			///< POE3 / OEI2
			OEI3,			///< POE3 / OEI3
			OEI4,			///< POE3 / OEI4
			TEI0,			///< RIIC0 / TEI0
			EEI0,			///< RIIC0 / EEI0
			TEI2,			///< RIIC2 / TEI2
			EEI2,			///< RIIC2 / EEI2
			SSIF0,			///< SSI0 / SSIF0
			SSIF1,			///< SSI1 / SSIF1

			S12CMPAI = 20,	///< S12AD / S12CMPAI
			S12CMPAI1 = 22,	///< S12AD1 / S12CMPAI1
		};


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  GROUPAL0・ベクター型
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class VECTOR_AL0 : uint8_t {
			TEIF8 = 0,		///< SCIFA8 / TEIF8
			ERIF8,			///< SCIFA8 / ERIF8
			BRIF8,			///< SCIFA8 / BRIF8
			DRIF8,			///< SCIFA8 / DRIF8
			TEIF9,			///< SCIFA9 / TEIF9
			ERIF9,			///< SCIFA9 / ERIF9
			BRIF9,			///< SCIFA9 / BRIF9
			DRIF9,			///< SCIFA9 / DRIF9

			TEIF10 = 8,		///< SCIFA10 / TEIF10
			ERIF10,			///< SCIFA10 / ERIF10
			BRIF10,			///< SCIFA10 / BRIF10
			DRIF10,			///< SCIFA10 / DRIF10
			TEIF11,			///< SCIFA11 / TEIF11
			ERIF11,			///< SCIFA11 / ERIF11
			BRIF11,			///< SCIFA11 / BRIF11
			DRIF11,			///< SCIFA11 / DRIF11

			SPII0 = 16,		///< RSPI0 / SPII0
			SPEI0,			///< RSPI0 / SPEI0
			SPII1,			///< RSPI1 / SPII1
			SPEI1,			///< RSPI1 / SPEI1
		};


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  GROUPAL1・ベクター型
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		enum class VECTOR_AL1 : uint8_t {
			MINT = 0,		///< EPTPC / MINT
			PINT,			///< PTPEDMAC / PINT

			EINT0 = 4,		///< EDMAC0/EINT0
			EINT1 = 5,		///< EDMAC1/EINT1
		};


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  グループベクター型の取得
			@param[in]	VEC		グループ BL0 ベクター型
			@return グループ BL0 ベクター型
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static VECTOR get_group_vector(VECTOR_BL0 ivec) noexcept {
			return VECTOR::GROUPBL0;
		}


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  グループベクター型の取得
			@param[in]	VEC		グループ BL1 ベクター型
			@return グループ BL1 ベクター型
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static VECTOR get_group_vector(VECTOR_BL1 ivec) noexcept {
			return VECTOR::GROUPBL1;
		}


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  グループベクター型の取得
			@param[in]	VEC		グループ BE0 ベクター型
			@return グループ BE0 ベクター型
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static VECTOR get_group_vector(VECTOR_BE0 ivec) noexcept {
			return VECTOR::GROUPBE0;
		}


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  グループベクター型の取得
			@param[in]	VEC		グループ AL0 ベクター型
			@return グループ AL0 ベクター型
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static VECTOR get_group_vector(VECTOR_AL0 ivec) noexcept {
			return VECTOR::GROUPAL0;
		}


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  グループベクター型の取得
			@param[in]	VEC		グループ AL1 ベクター型
			@return グループ AL1 ベクター型
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static VECTOR get_group_vector(VECTOR_AL1 ivec) noexcept {
			return VECTOR::GROUPAL1;
		}


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  IR レジスタ・クラス
			@param[in]	base	ベースアドレス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <uint32_t base>
		struct ir_t {

			//-------------------------------------------------------------//
			/*!
				@brief  []オペレータ
				@param[in]	vec		割り込みベクター型
				@return IR レジスターの参照
			*/
			//-------------------------------------------------------------//
			volatile uint8_t& operator [] (VECTOR vec) noexcept {
				return *reinterpret_cast<volatile uint8_t*>(base + static_cast<uint8_t>(vec));
			}
		};
		typedef ir_t<0x00087000> IR_;
		static IR_ IR;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  IER レジスタ・クラス
			@param[in]	base	ベースアドレス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <uint32_t base>
		struct ier_t {

			//-------------------------------------------------------------//
			/*!
				@brief  許可、不許可
				@param[in]	vec		割り込みベクター型
				@param[in]	ena		不許可の場合「false」
			*/
			//-------------------------------------------------------------//
			void enable(VECTOR vec, bool ena = true) noexcept
			{
				auto idx = static_cast<uint8_t>(vec);
				auto tmp = rd8_(base + (idx >> 3));
				if(ena) {
					tmp |=   1 << (idx & 7);
				} else {
					tmp &= ~(1 << (idx & 7));
				}
				wr8_(base + (idx >> 3), tmp);
			}


			//-------------------------------------------------------------//
			/*!
				@brief  許可状態を取得
				@param[in]	vec		割り込みベクター型
				@return 許可状態（許可の場合「true」）
			*/
			//-------------------------------------------------------------//
			bool get(VECTOR vec) const noexcept
			{
				auto idx = static_cast<uint8_t>(vec);
				auto tmp = rd8_(base + (idx >> 3));
				return tmp & (1 << (idx & 7));
			}
		};
		typedef ier_t<0x00087200> IER_;
		static IER_ IER;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  IPR レジスタ・クラス @n
					全て、下位４ビットが有効
			@param[in]	base	ベースアドレス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <uint32_t base>
		struct ipr_t {

			//-------------------------------------------------------------//
			/*!
				@brief  []オペレータ
				@param[in]	vec		標準割り込みベクター型
				@return IPR レジスターの参照
			*/
			//-------------------------------------------------------------//
			volatile uint8_t& operator [] (VECTOR vec) {
				uint32_t idx = 0;
				switch(vec) {
				case VECTOR::BUSERR: idx = 0; break;
				case VECTOR::RAMERR: idx = 0; break;
				case VECTOR::FIFERR: idx = 1; break;
				case VECTOR::FRDYI:  idx = 2; break;
				case VECTOR::SWINT2: idx = 3; break;
				case VECTOR::SWINT:  idx = 3; break;
				case VECTOR::CMI0:   idx = 4; break;
				case VECTOR::CMI1:   idx = 5; break;
				case VECTOR::CMWI0:  idx = 6; break;
				case VECTOR::CMWI1:  idx = 7; break;
				default: idx = static_cast<uint32_t>(vec); break;
				}
				return *reinterpret_cast<volatile uint8_t*>(base + idx);
			}
		};
		typedef ipr_t<0x00087300> IPR_;
		static IPR_ IPR;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  グループ割り込み要求レジスタ
			@param[in]	base	ベースアドレス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <uint32_t base>
		struct grp_t : public ro32_t<base> {
			typedef ro32_t<base> io_;
			using io_::operator ();

			bit_ro_t <io_, bitpos::B0>  IS0;
			bit_ro_t <io_, bitpos::B1>  IS1;
			bit_ro_t <io_, bitpos::B2>  IS2;
			bit_ro_t <io_, bitpos::B3>  IS3;
			bit_ro_t <io_, bitpos::B4>  IS4;
			bit_ro_t <io_, bitpos::B5>  IS5;
			bit_ro_t <io_, bitpos::B6>  IS6;
			bit_ro_t <io_, bitpos::B7>  IS7;

			bit_ro_t <io_, bitpos::B8>  IS8;
			bit_ro_t <io_, bitpos::B9>  IS9;
			bit_ro_t <io_, bitpos::B10> IS10;
			bit_ro_t <io_, bitpos::B11> IS11;
			bit_ro_t <io_, bitpos::B12> IS12;
			bit_ro_t <io_, bitpos::B13> IS13;
			bit_ro_t <io_, bitpos::B14> IS14;
			bit_ro_t <io_, bitpos::B15> IS15;

			bit_ro_t <io_, bitpos::B16> IS16;
			bit_ro_t <io_, bitpos::B17> IS17;
			bit_ro_t <io_, bitpos::B18> IS18;
			bit_ro_t <io_, bitpos::B19> IS19;
			bit_ro_t <io_, bitpos::B20> IS20;
			bit_ro_t <io_, bitpos::B21> IS21;
			bit_ro_t <io_, bitpos::B22> IS22;
			bit_ro_t <io_, bitpos::B23> IS23;

			bit_ro_t <io_, bitpos::B24> IS24;
			bit_ro_t <io_, bitpos::B25> IS25;
			bit_ro_t <io_, bitpos::B26> IS26;
			bit_ro_t <io_, bitpos::B27> IS27;
			bit_ro_t <io_, bitpos::B28> IS28;
			bit_ro_t <io_, bitpos::B29> IS29;
			bit_ro_t <io_, bitpos::B30> IS30;
			bit_ro_t <io_, bitpos::B31> IS31;
		};


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  グループ BE0 割り込み要求レジスタ（GRPBE0）@n
					グループ BL0/1 割り込み要求レジスタ（GRPBL0/GRPBL1）@n
					グループ AL0/1 割り込み要求レジスタ（GRPAL0/GRPAL1）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef grp_t<0x00087600> GRPBE0_;
		static GRPBE0_ GRPBE0;
		typedef grp_t<0x00087630> GRPBL0_;
		static GRPBL0_ GRPBL0;
		typedef grp_t<0x00087634> GRPBL1_;
		static GRPBL1_ GRPBL1;
		typedef grp_t<0x00087830> GRPAL0_;
		static GRPAL0_ GRPAL0;
		typedef grp_t<0x00087834> GRPAL1_;
		static GRPAL1_ GRPAL1;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  グループ割り込み要求許可レジスタ
			@param[in]	base	ベースアドレス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <uint32_t base>
		struct gen_t : public rw32_t<base> {
			typedef rw32_t<base> io_;
			using io_::operator =;
			using io_::operator ();
			using io_::operator |=;
			using io_::operator &=;

			bit_rw_t<io_, bitpos::B0>  EN0;
			bit_rw_t<io_, bitpos::B1>  EN1;
			bit_rw_t<io_, bitpos::B2>  EN2;
			bit_rw_t<io_, bitpos::B3>  EN3;
			bit_rw_t<io_, bitpos::B4>  EN4;
			bit_rw_t<io_, bitpos::B5>  EN5;
			bit_rw_t<io_, bitpos::B6>  EN6;
			bit_rw_t<io_, bitpos::B7>  EN7;

			bit_rw_t<io_, bitpos::B8>  EN8;
			bit_rw_t<io_, bitpos::B9>  EN9;
			bit_rw_t<io_, bitpos::B10> EN10;
			bit_rw_t<io_, bitpos::B11> EN11;
			bit_rw_t<io_, bitpos::B12> EN12;
			bit_rw_t<io_, bitpos::B13> EN13;
			bit_rw_t<io_, bitpos::B14> EN14;
			bit_rw_t<io_, bitpos::B15> EN15;

			bit_rw_t<io_, bitpos::B16> EN16;
			bit_rw_t<io_, bitpos::B17> EN17;
			bit_rw_t<io_, bitpos::B18> EN18;
			bit_rw_t<io_, bitpos::B19> EN19;
			bit_rw_t<io_, bitpos::B20> EN20;
			bit_rw_t<io_, bitpos::B21> EN21;
			bit_rw_t<io_, bitpos::B22> EN22;
			bit_rw_t<io_, bitpos::B23> EN23;

			bit_rw_t<io_, bitpos::B24> EN24;
			bit_rw_t<io_, bitpos::B25> EN25;
			bit_rw_t<io_, bitpos::B26> EN26;
			bit_rw_t<io_, bitpos::B27> EN27;
			bit_rw_t<io_, bitpos::B28> EN28;
			bit_rw_t<io_, bitpos::B29> EN29;
			bit_rw_t<io_, bitpos::B30> EN30;
			bit_rw_t<io_, bitpos::B31> EN31;
		};


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  グループ BE0 割り込み要求許可レジスタ（GENBE0）@n
					グループ BL0/1 割り込み要求許可レジスタ（GENBL0/GENBL1/GENBL2）@n
					グループ AL0/1 割り込み要求許可レジスタ（GENAL0/GENAL1）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef gen_t<0x00087640> GENBE0_;
		static GENBE0_ GENBE0;
		typedef gen_t<0x00087670> GENBL0_;
		static GENBL0_ GENBL0;
		typedef gen_t<0x00087674> GENBL1_;
		static GENBL1_ GENBL1;
		typedef gen_t<0x00087870> GENAL0_;
		static GENAL0_ GENAL0;
		typedef gen_t<0x00087874> GENAL1_;
		static GENAL1_ GENAL1;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  グループ BE0 割り込みクリアレジスタ（GCRBE0）
			@param[in]	base	ベースアドレス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <uint32_t base>
		struct gcrbe0_t : public rw32_t<base> {
			typedef rw32_t<base> io_;
			using io_::operator =;
			using io_::operator ();
			using io_::operator |=;
			using io_::operator &=;

			bit_rw_t <io_, bitpos::B0>  CLR0;
			bit_rw_t <io_, bitpos::B1>  CLR1;
			bit_rw_t <io_, bitpos::B2>  CLR2;
			bit_rw_t <io_, bitpos::B3>  CLR3;
			bit_rw_t <io_, bitpos::B4>  CLR4;
			bit_rw_t <io_, bitpos::B5>  CLR5;
			bit_rw_t <io_, bitpos::B6>  CLR6;
			bit_rw_t <io_, bitpos::B7>  CLR7;

			bit_rw_t <io_, bitpos::B8>  CLR8;
			bit_rw_t <io_, bitpos::B9>  CLR9;
			bit_rw_t <io_, bitpos::B10> CLR10;
			bit_rw_t <io_, bitpos::B11> CLR11;
			bit_rw_t <io_, bitpos::B12> CLR12;
			bit_rw_t <io_, bitpos::B13> CLR13;
			bit_rw_t <io_, bitpos::B14> CLR14;
			bit_rw_t <io_, bitpos::B15> CLR15;

			bit_rw_t <io_, bitpos::B16> CLR16;
			bit_rw_t <io_, bitpos::B17> CLR17;
			bit_rw_t <io_, bitpos::B18> CLR18;
			bit_rw_t <io_, bitpos::B19> CLR19;
			bit_rw_t <io_, bitpos::B20> CLR20;
			bit_rw_t <io_, bitpos::B21> CLR21;
			bit_rw_t <io_, bitpos::B22> CLR22;
			bit_rw_t <io_, bitpos::B23> CLR23;

			bit_rw_t <io_, bitpos::B24> CLR24;
			bit_rw_t <io_, bitpos::B25> CLR25;
			bit_rw_t <io_, bitpos::B26> CLR26;
			bit_rw_t <io_, bitpos::B27> CLR27;
			bit_rw_t <io_, bitpos::B28> CLR28;
			bit_rw_t <io_, bitpos::B29> CLR29;
			bit_rw_t <io_, bitpos::B30> CLR30;
			bit_rw_t <io_, bitpos::B31> CLR31;
		};
		typedef gcrbe0_t<0x00087680> GCRBE0_;
		static GCRBE0_ GCRBE0;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  選択型割り込み要求レジスタ
			@param[in]	base	ベースアドレス
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		template <uint32_t base>
		struct pixr_t : public rw8_t<base> {
			typedef rw8_t<base> io_;
			using io_::operator =;
			using io_::operator ();
			using io_::operator |=;
			using io_::operator &=;

			bit_rw_t <io_, bitpos::B0>  PIR0;
			bit_rw_t <io_, bitpos::B1>  PIR1;
			bit_rw_t <io_, bitpos::B2>  PIR2;
			bit_rw_t <io_, bitpos::B3>  PIR3;
			bit_rw_t <io_, bitpos::B4>  PIR4;
			bit_rw_t <io_, bitpos::B5>  PIR5;
			bit_rw_t <io_, bitpos::B6>  PIR6;
			bit_rw_t <io_, bitpos::B7>  PIR7;
		};


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  選択型割り込み B 要求レジスタ k（PIBRk）（k = 0h ～ Ah）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static pixr_t<0x00087700> PIBR0;
		static pixr_t<0x00087701> PIBR1;
		static pixr_t<0x00087702> PIBR2;
		static pixr_t<0x00087703> PIBR3;
		static pixr_t<0x00087704> PIBR4;
		static pixr_t<0x00087705> PIBR5;
		static pixr_t<0x00087706> PIBR6;
		static pixr_t<0x00087707> PIBR7;
		static pixr_t<0x00087708> PIBR8;
		static pixr_t<0x00087709> PIBR9;
		static pixr_t<0x0008770A> PIBRA;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  選択型割り込み A 要求レジスタ k（PIARk）（k = 0h ～ Bh）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		static pixr_t<0x00087900> PIAR0;
		static pixr_t<0x00087901> PIAR1;
		static pixr_t<0x00087902> PIAR2;
		static pixr_t<0x00087903> PIAR3;
		static pixr_t<0x00087904> PIAR4;
		static pixr_t<0x00087905> PIAR5;
		static pixr_t<0x00087906> PIAR6;
		static pixr_t<0x00087907> PIAR7;
		static pixr_t<0x00087908> PIAR8;
		static pixr_t<0x00087909> PIAR9;
		static pixr_t<0x0008790A> PIARA;
		static pixr_t<0x0008790B> PIARB;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  選択型割り込み B 要因選択レジスタ Xn（SLIBXRn）（n = 128 ～ 143）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef icu_utils::slixr_t<0x00087700, VECTOR, VECTOR_SELB> SLIBR_;
		static SLIBR_ SLIBR;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		/*!
			@brief  選択型割り込み A 要因選択レジスタ n（SLIARn）（n = 208 ～ 255）
		*/
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
		typedef icu_utils::slixr_t<0x00087900, VECTOR, VECTOR_SELA> SLIAR_;
		static SLIAR_ SLIAR;
	};
	typedef icu_t<void> ICU;

	// テンプレート内、スタティック定義、実態：
	template<class _> typename icu_t<_>::IR_  icu_t<_>::IR;
	template<class _> typename icu_t<_>::IER_ icu_t<_>::IER;
	template<class _> typename icu_t<_>::IPR_ icu_t<_>::IPR;

	template<class _> typename icu_t<_>::GRPBE0_ icu_t<_>::GRPBE0;
	template<class _> typename icu_t<_>::GRPBL0_ icu_t<_>::GRPBL0;
	template<class _> typename icu_t<_>::GRPBL1_ icu_t<_>::GRPBL1;
	template<class _> typename icu_t<_>::GRPAL0_ icu_t<_>::GRPAL0;
	template<class _> typename icu_t<_>::GRPAL1_ icu_t<_>::GRPAL1;

	template<class _> typename icu_t<_>::GENBE0_ icu_t<_>::GENBE0;
	template<class _> typename icu_t<_>::GENBL0_ icu_t<_>::GENBL0;
	template<class _> typename icu_t<_>::GENBL1_ icu_t<_>::GENBL1;
	template<class _> typename icu_t<_>::GENAL0_ icu_t<_>::GENAL0;
	template<class _> typename icu_t<_>::GENAL1_ icu_t<_>::GENAL1;

	template<class _> typename icu_t<_>::GCRBE0_ icu_t<_>::GCRBE0;

	template<class _> typename icu_t<_>::SLIBR_ icu_t<_>::SLIBR;
	template<class _> typename icu_t<_>::SLIAR_ icu_t<_>::SLIAR;
}
