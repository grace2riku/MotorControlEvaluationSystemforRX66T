//=====================================================================//
/*! @file
    @brief  EFO 2ND (RX64M) メイン @n
			・デバッグ用LED出力 ---> PE3(108) @n
			・デバッグ用シリアルＲＸＤ  --->  P21(36) / RXD0 @n
			・デバッグ用シリアルＴＸＤ  --->  P20(37) / TXD0 @n
			・変換トリガー閾値（コンパレーター入力+へ)   ---> P03(4) / DA0  @n
			・変換トリガー入力（コンパレーター出力から） ---> PF5(9) / IRQ4
	@copyright Copyright 2018 Kunihito Hiramatsu All Right Reserved.
    @author 平松邦仁 (hira@rvf-rc45.net)
*/
//=====================================================================//
#include "common/renesas.hpp"
#include "common/cmt_mgr.hpp"
#include "common/tpu_io.hpp"
#include "common/fixed_fifo.hpp"
#include "common/sci_io.hpp"
#include "common/spi_io.hpp"
#include "common/rspi_io.hpp"
#include "chip/LTC2348_16a.hpp"

// #define UART_DEBUG

namespace {

	static const int main_version_ = 104;
	static const uint32_t build_id_ = B_ID;

	enum class CMD : uint8_t {
		START       = 0x01,
		END         = 0x03,
		MODE_SELECT = 0x11,
		MODE_SENSE  = 0x12,
		VER         = 0x21,

//		OFFSET = 'O';			///< トリガーオフセット、'O', L, H 

		BIN_CNVTESTS = 'B',		///< １０２４キャプチャーバイナリ転送
		BIN_CNVTEST  = 'b',		///< １キャプチャーバイナリ転送

		ASC_CNVTESTS = 'T',		///< １０２４キャプチャー、文字表示
		ASC_CNVTEST  = 't',		///< １キャプチャー、文字表示

		TEST_TRG_N = 'Q',		///< テストトリガ立ち下がり
		TEST_TRG_P = 'q'		///< テストトリガ立ち上がり
	};

	enum class TASK : uint8_t {
		STATE,		// コマンド待ち
		LENGTH,		// 長さ取得
		VOLT,		// トリガー電圧取得
		TRG,		// トリガー
	};

	enum class SEND_TASK : uint8_t {
		READY,
		SINGLE,
		MULTI,

		TRG,

		ASCII_SINGLE,	///< t コマンド（１サンプルテスト）
		ASCII_MULTI,	///< T コマンド（１０２４サンプルテスト）

		BIN_SINGLE,		///< b コマンド（１サンプルテスト）
		BIN_MULTI,		///< B コマンド（１０２４サンプルテスト）
	};

	typedef device::PORT<device::PORTF, device::bitpos::B5> TRIGGER;

	typedef device::PORT<device::PORTE, device::bitpos::B3> LED;

	typedef device::cmt_mgr<device::CMT0, utils::null_task> CMT;
	CMT		cmt_;

	// LTC2348-16 A/D 制御ポート定義
	// LTC2348ILX-16 (38:BUSY) ---> P16(40/144) / IRQ6
	// LTC2348ILX-16 (24:CNV)  ---> PB3(82/144) / MTIOC0A
	// LTC2348ILX-16 (29:SCKI) ---> PA5(90/144) / RSPCKA-B
	// LTC2348ILX-16 (37:SDI)  ---> PA6(89/144) / MOSIA-B
	// LTC2348ILX-16 (25:SDO0) ---> PA7(88/144) / MISOA-B
	typedef device::PORT<device::PORT4, device::bitpos::B0> LTC_CSN;   // P40(141)
	typedef device::PORT<device::PORTB, device::bitpos::B3> LTC_CNV;   // PB3(82)
	typedef device::PORT<device::PORT1, device::bitpos::B6> LTC_BUSY;
	typedef device::PORT<device::PORT5, device::bitpos::B3> LTC_PD;    // P53(53)

// #define SOFT_SPI
#ifdef SOFT_SPI
#if 0
//  旧タイプハード、SCI 簡易 SPI タイプ
	typedef device::PORT<device::PORT5, device::bitpos::B1> SPCK;
	typedef device::PORT<device::PORT5, device::bitpos::B2> MISO;
	typedef device::PORT<device::PORT5, device::bitpos::B0> MOSI;
#else
	typedef device::PORT<device::PORTA, device::bitpos::B5> SPCK;
	typedef device::PORT<device::PORTA, device::bitpos::B7> MISO;
	typedef device::PORT<device::PORTA, device::bitpos::B6> MOSI;
#endif
	typedef device::spi_io<MISO, MOSI, SPCK, device::soft_spi_mode::LTC> SPI_IO;
#else
	// RSPI ハード（ポートの割り当ては、port_map クラスを参照）
	typedef device::rspi_io<device::RSPI0, device::port_map::ORDER::SECOND> SPI_IO;
#endif
	// LTC2348_16a: EFO 専用、サンプリング方法によるドライバー
	typedef chip::LTC2348_16a<LTC_CSN, LTC_CNV, LTC_BUSY, SPI_IO> EADC;
	EADC		eadc_;

	// バッファ・サイズは２のｎ乗のみ設定可能
	static const uint32_t CAP_BUFF_SIZE = 4096;
	uint16_t ch0_src_[CAP_BUFF_SIZE];
	uint16_t ch1_src_[CAP_BUFF_SIZE];

	volatile uint16_t cap_pos_ = 0;
	volatile uint16_t cap_cnt_ = 0;
	volatile uint16_t trg_pos_ = 0;
	volatile uint16_t req_cnt_ = 0;
	volatile bool cap_enable_ = false;
	volatile bool trg_enable_ = false;
	volatile bool trg_slope_ = true;

	bool trigger_ = false;

	// サンプリング割り込み起動のファンクタ
	class capture_task {
	public:
		void operator() ()
		{
			if(cap_enable_) {
///				LED::P = 1;
				eadc_.convert_0();

				bool f = TRIGGER::P();
				bool trg = false;
				if(trg_slope_) {
					if( f && !trigger_) {  // pos
						trg = true;
					}
				} else {
					if(!f &&  trigger_) {  // neg
						trg = true;
					}
				}
				trigger_ = f;
				if(cap_cnt_ == 0 && trg && trg_enable_) {
					trg_pos_ = cap_pos_;
					cap_cnt_ = req_cnt_;
				}

				++cap_pos_;
				cap_pos_ &= CAP_BUFF_SIZE - 1;
				ch0_src_[cap_pos_] = eadc_.convert_1();

				if(cap_cnt_) {
					--cap_cnt_;
					if(cap_cnt_ == 0) {
						cap_enable_ = false;
						trg_enable_ = false;
					}
				}

				ch1_src_[cap_pos_] = eadc_.convert_2();
///				LED::P = 0;
			}
		}
	};

	typedef device::tpu_io<device::TPU0, capture_task> TPU0;
	TPU0		tpu0_;

#ifdef UART_DEBUG
	typedef device::PORT<device::PORTF, device::bitpos::B5> TRG;

	typedef utils::fixed_fifo<char, 1024> BUFFER;
	typedef device::sci_io<device::SCI12, BUFFER, BUFFER> D_SCI;
	D_SCI			d_sci_;
#else
	typedef utils::fixed_fifo<char, 1024> BUFFER;
	typedef device::sci_io<device::SCI0,  BUFFER, BUFFER> D_SCI;
	D_SCI			d_sci_;

	typedef utils::fixed_fifo<char, 4096> SENDBUF;
	typedef device::sci_io<device::SCI12, BUFFER, SENDBUF> M_SCI;
	M_SCI			m_sci_;
#endif

	typedef device::dac_out<device::R12DA> DAC;
	DAC		dac_;

	uint16_t ch0_trg_[1024];
	uint16_t ch1_trg_[1024];

	void copy_(uint32_t org, uint32_t len)
	{
		for(uint32_t i = 0; i < len; ++i) {
			org &= (CAP_BUFF_SIZE - 1);
			ch0_trg_[i] = ch0_src_[org];
			ch1_trg_[i] = ch1_src_[org];
			++org;
		}
	}

#if 0
	volatile uint32_t irq_count_;

	// trigger task
	class irq_task {
	public:
		void operator() ()
		{
			cap_cnt_ = req_cnt_;
			trg_pos_ = cap_pos_;
			++irq_count_;
		}
	};

	typedef device::irq_man<device::peripheral::IRQ4, irq_task> IRQ;
	IRQ		irq_;
#endif

	TASK	task_ = TASK::STATE;

	uint16_t	length_;
	uint16_t	volt_;
	uint8_t		edge_;

	uint32_t	irq_state_;

	SEND_TASK  	send_task_;


	void send_wave_legacy_(char ch, uint16_t len, const uint16_t* src)
	{
		m_sci_.putch(0x01);  // A/D シングル通知
		m_sci_.putch(ch);    // chanel NO
		for(uint16_t i = 0; i < len; ++i) {
			if(m_sci_.recv_length() > 0) {
				if(static_cast<CMD>(m_sci_.getch()) == CMD::END) {  // END command
					return;
				}
			}
			m_sci_.putch(src[i] >> 8);
			m_sci_.putch(src[i] & 0xff);
		}
	}


	void send_wave_(char ch, uint16_t len, const uint16_t* src)
	{
		m_sci_.putch(0x02);  // A/D マルチ通知
		m_sci_.putch(ch);    // chanel NO
		m_sci_.putch(len >> 8);
		m_sci_.putch(len & 0xff);
		for(uint16_t i = 0; i < len; ++i) {
			if(m_sci_.recv_length() > 0) {
				if(static_cast<CMD>(m_sci_.getch()) == CMD::END) {  // END command
					return;
				}
			}
			m_sci_.putch(src[i] >> 8);
			m_sci_.putch(src[i] & 0xff);
		}
	}

#if 0
	void setup_irq_(bool positive)
	{
		uint8_t int_level = 2;

		IRQ::edge edge = IRQ::edge::POSITIVE;
		if(!positive) edge = IRQ::edge::NEGATIVE;
		bool ret = irq_.start(int_level, edge, device::port_map::ORDER::SECOND);
		if(!ret) {
			utils::format("IRQ start fail...\n");
			return;
		}
	}
#endif
}


extern "C" {

	//-----------------------------------------------------------------//
	/*!
		@brief  システム・文字出力
		@param[in]	ch	文字
	*/
	//-----------------------------------------------------------------//
	void sci_putch(char ch)
	{
#if 0
		if(putch_lock_) {
			if((putch_tmp_.size() - putch_tmp_.length()) >= 2) {
				putch_tmp_.put(ch);
			}
			return;
		}
		putch_lock_ = true;
#endif
		d_sci_.putch(ch);
//		putch_lock_ = false;
	}


	//-----------------------------------------------------------------//
	/*!
		@brief  システム・文字列出力
		@param[in]	s	文字列
	*/
	//-----------------------------------------------------------------//
	void sci_puts(const char* s)
	{
		d_sci_.puts(s);
	}


	//-----------------------------------------------------------------//
	/*!
		@brief  システム・文字入力
		@return	文字
	*/
	//-----------------------------------------------------------------//
	char sci_getch(void)
	{
		return d_sci_.getch();
	}


	//-----------------------------------------------------------------//
	/*!
		@brief  システム・文字列長の取得
		@return	文字列長
	*/
	//-----------------------------------------------------------------//
	uint16_t sci_length(void)
	{
		return d_sci_.recv_length();
	}
}


int main(int argc, char** argv);

int main(int argc, char** argv)
{
	device::PORT3::PCR.B5 = 1; // P35(NMI) pull-up

	device::PORT0::PDR.B5 = 1;  // (2)
	device::PORT0::PDR.B2 = 1;  // (6)
	device::PORT0::PDR.B1 = 1;  // (7)
	device::PORT0::PDR.B0 = 1;  // (8)
	device::PORTJ::PDR.B5 = 1;  // (11)
	device::PORTJ::PDR.B3 = 1;  // (13)

	device::PORT3::PDR.B3 = 1;  // (26)
	device::PORT3::PDR.B2 = 1;  // (27)

	device::PORT2::PDR.B5 = 1;  // (32)
	device::PORT2::PDR.B4 = 1;  // (33)
	device::PORT2::PDR.B3 = 1;  // (34)
	device::PORT2::PDR.B2 = 1;  // (35)
	device::PORT2::PDR.B1 = 1;  // (36)
	device::PORT2::PDR.B0 = 1;  // (37)
	device::PORT1::PDR.B7 = 1;  // (38)
	device::PORT8::PDR.B7 = 1;  // (39)

	device::PORT8::PDR.B6 = 1;  // (41)
	device::PORT1::PDR.B5 = 1;  // (42)
	device::PORT1::PDR.B4 = 1;  // (43)
	device::PORT1::PDR.B3 = 1;  // (44)
	device::PORT1::PDR.B2 = 1;  // (45)

	device::PORT5::PDR.B6 = 1;  // (50)
	device::PORT5::PDR.B5 = 1;  // (51)
	device::PORT5::PDR.B4 = 1;  // (52)

	device::PORT5::PDR.B2 = 1;  // (54)
	device::PORT5::PDR.B1 = 1;  // (55)
	device::PORT5::PDR.B0 = 1;  // (56)

	device::PORT8::PDR.B3 = 1;  // (58)

	device::PORTC::PDR.B6 = 1;  // (61)
	device::PORTC::PDR.B5 = 1;  // (62)
	device::PORT8::PDR.B2 = 1;  // (63)
	device::PORT8::PDR.B1 = 1;  // (64)
	device::PORT8::PDR.B0 = 1;  // (65)
	device::PORTC::PDR.B4 = 1;  // (66)
	device::PORTC::PDR.B3 = 1;  // (67)
	device::PORT7::PDR.B7 = 1;  // (68)
	device::PORT7::PDR.B6 = 1;  // (69)
	device::PORTC::PDR.B2 = 1;  // (70)
	device::PORT7::PDR.B5 = 1;  // (71)
	device::PORT7::PDR.B4 = 1;  // (72)
	device::PORTC::PDR.B1 = 1;  // (73)

	device::PORTC::PDR.B0 = 1;  // (75)
	device::PORT7::PDR.B3 = 1;  // (77)
	device::PORTB::PDR.B7 = 1;  // (78)
	device::PORTB::PDR.B6 = 1;  // (79)
	device::PORTB::PDR.B5 = 1;  // (80)
	device::PORTB::PDR.B4 = 1;  // (81)

	device::PORTB::PDR.B2 = 1;  // (83)
	device::PORTB::PDR.B1 = 1;  // (84)
	device::PORT7::PDR.B2 = 1;  // (85)
	device::PORT7::PDR.B1 = 1;  // (86)
	device::PORTB::PDR.B0 = 1;  // (87)

	device::PORTA::PDR.B4 = 1;  // (92)

	device::PORTA::PDR.B3 = 1;  // (94)
	device::PORTA::PDR.B2 = 1;  // (95)
	device::PORTA::PDR.B1 = 1;  // (96)
	device::PORTA::PDR.B0 = 1;  // (97)
	device::PORT6::PDR.B7 = 1;  // (98)
	device::PORT6::PDR.B6 = 1;  // (99)
	device::PORT6::PDR.B5 = 1;  // (100)
	device::PORTE::PDR.B7 = 1;  // (101)
	device::PORTE::PDR.B6 = 1;  // (102)

	device::PORT7::PDR.B0 = 1;  // (104)

	device::PORTE::PDR.B5 = 1;  // (106)
	device::PORTE::PDR.B4 = 1;  // (107)

	device::PORTE::PDR.B0 = 1;  // (111)
	device::PORT6::PDR.B4 = 1;  // (112)
	device::PORT6::PDR.B3 = 1;  // (113)
	device::PORT6::PDR.B2 = 1;  // (114)
	device::PORT6::PDR.B1 = 1;  // (115)

	device::PORT6::PDR.B0 = 1;  // (117)

	device::PORTD::PDR.B7 = 1;  // (119)
	device::PORTD::PDR.B6 = 1;  // (120)
	device::PORTD::PDR.B5 = 1;  // (121)
	device::PORTD::PDR.B4 = 1;  // (122)
	device::PORTD::PDR.B3 = 1;  // (123)
	device::PORTD::PDR.B2 = 1;  // (124)
	device::PORTD::PDR.B1 = 1;  // (125)
	device::PORTD::PDR.B0 = 1;  // (126)

	device::PORT9::PDR.B3 = 1;  // (127)
	device::PORT9::PDR.B2 = 1;  // (128)
	device::PORT9::PDR.B1 = 1;  // (129)

	device::PORT9::PDR.B0 = 1;  // (131)

	device::PORT4::PDR.B7 = 1;  // (133)
	device::PORT4::PDR.B6 = 1;  // (134)
	device::PORT4::PDR.B5 = 1;  // (135)
	device::PORT4::PDR.B4 = 1;  // (136)
	device::PORT4::PDR.B3 = 1;  // (137)
	device::PORT4::PDR.B2 = 1;  // (138)
	device::PORT4::PDR.B1 = 1;  // (139)

	device::PORT0::PDR.B7 = 1;  // (144)

	SYSTEM_IO::boost_master_clock();

	LED::DIR = 1;

	{
		// タイマー設定（100Hz）
		auto cmt_irq_level = device::ICU::LEVEL::_3;
		cmt_.start(100, cmt_irq_level);
	}

	{  // TPU0 設定
		auto int_level = device::ICU::LEVEL::_4;
		if(!tpu0_.start(375000, int_level)) {
			utils::format("TPU0 not start ...\n");
		}
	}

	{  // DEBUG SCI 設定
		auto int_level = device::ICU::LEVEL::_1;
		d_sci_.start(115200, int_level);
	}

	{  // MAIN SCI 設定
		auto int_level = device::ICU::LEVEL::_1;
		m_sci_.start(115200, int_level);
		m_sci_.auto_crlf(false);
	}

	{  // D/A CH0, AMP enable
		dac_.start(DAC::output::CH0, true);
	}

	{  // LTC2348ILX-16 初期化
		LTC_PD::DIR = 1;
		LTC_PD::P = 0;  // パワーダウンしない！
		// 内臓リファレンスと内臓バッファ
		// VREFIN: 2.024V、VREFBUF: 4.096V、Analog range: -10.24V to +10.24V
		if(!eadc_.start(800000, EADC::span_type::M10_24P10_24)) {
			utils::format("LTC2348_16 not found...\n");
		}
	}

#if 0
	irq_count_ = 0;
	irq_state_ = 0;
#endif

	cap_pos_ = 0;
	cap_cnt_ = 0;
	cap_enable_ = false;

	// 初期設定
	dac_.out0(32768);  // トリガー電圧０Ｖ

	uint8_t cnt = 0;
	while(1) {
		cmt_.sync();

		switch(task_) {
		case TASK::STATE:
			if(m_sci_.recv_length() >= 1) {
				CMD cmd = static_cast<CMD>(m_sci_.getch());
				switch(cmd) {
				case CMD::START:
					cap_enable_ = true;
					task_ = TASK::LENGTH;
					break;
				case CMD::ASC_CNVTEST:
					cap_enable_ = true;
					send_task_ = SEND_TASK::ASCII_SINGLE;
					break;
				case CMD::ASC_CNVTESTS:
					cap_enable_ = true;
					send_task_ = SEND_TASK::ASCII_MULTI;
					break;
				case CMD::BIN_CNVTEST:
					cap_enable_ = true;
					send_task_ = SEND_TASK::BIN_SINGLE;
					break;
				case CMD::BIN_CNVTESTS:
					cap_enable_ = true;
					send_task_ = SEND_TASK::BIN_MULTI;
					break;
				case CMD::END:
					if(send_task_ == SEND_TASK::MULTI) {  // 波形取得を強制終了
						send_task_ = SEND_TASK::READY;
					}
					break;

				case CMD::TEST_TRG_P:
					trg_slope_ = true;
					req_cnt_ = 1024;
					trg_enable_ = true;
					trigger_ = TRIGGER::P();
					cap_enable_ = true;
					send_task_ = SEND_TASK::TRG;
					break;
				case CMD::TEST_TRG_N:
					trg_slope_ = false;
					req_cnt_ = 1024;
					trg_enable_ = true;
					trigger_ = TRIGGER::P();
					cap_enable_ = true;
					send_task_ = SEND_TASK::TRG;
					break;

				case CMD::MODE_SELECT:
					break;

				case CMD::MODE_SENSE:
					break;

				case CMD::VER:
					m_sci_.putch(0x21);  // VERデータID
					m_sci_.putch(20);  // 電圧レンジ（+-20V）
					{
						char tmp[4];
						auto v = main_version_;
						tmp[3] = (v % 10) + '0';
						v /= 10;
						tmp[2] = (v % 10) + '0';
						v /= 10;
						tmp[1] = (v % 10) + '0';
						v /= 10;
						tmp[0] = (v % 10) + '0';
						m_sci_.putch(tmp[3]);
						m_sci_.putch(tmp[2]);
						m_sci_.putch(tmp[1]);
						m_sci_.putch(tmp[0]);
					}
					break;

				default:
					break;
				}
			}
			break;

		case TASK::LENGTH:
			if(m_sci_.recv_length() >= (2 + 1)) {
				length_  = static_cast<uint8_t>(m_sci_.getch());
				length_ <<= 8;
				length_ |= static_cast<uint8_t>(m_sci_.getch());
				task_ = TASK::VOLT;
			}
			break;

		case TASK::VOLT:
			if(m_sci_.recv_length() >= (2 + 1)) {
				volt_  = static_cast<uint8_t>(m_sci_.getch());
				volt_ <<= 8;
				volt_ |= static_cast<uint8_t>(m_sci_.getch());
				// start command 追加仕様（５月１４日、２０１８年）
				edge_ = static_cast<uint8_t>(m_sci_.getch());
				task_ = TASK::STATE;
				dac_.out0(volt_);  // トリガー電圧設定
///				if(volt_ < 32768) trg_slope_ = false;
///				else trg_slope_ = true;
				if(edge_ == 0) trg_slope_ = true;
				else trg_slope_ = false;
				if(length_ == 0) {
					cap_cnt_ = 1;
					trg_enable_ = false;
					cap_enable_ = true;
					send_task_ = SEND_TASK::SINGLE;
				} else {
					uint16_t n = length_;
					if(length_ > 1024) n = 1024;
					req_cnt_ = n;
					trg_enable_ = true;
					trigger_ = TRIGGER::P();
					cap_enable_ = true;
					send_task_ = SEND_TASK::MULTI;
				}
			}
			break;

		default:
			break;
		}

		switch(send_task_) {

		case SEND_TASK::SINGLE:
			copy_(cap_pos_, 1);
			send_wave_legacy_(0x01, 1, ch0_trg_);
			send_wave_legacy_(0x02, 1, ch1_trg_);
			send_task_ = SEND_TASK::READY;
			break;

		case SEND_TASK::MULTI:
			if(cap_cnt_ == 0 && !trg_enable_) ;
			else break;
			copy_(trg_pos_, length_);
			send_wave_legacy_(0x01, length_, ch0_trg_);
			send_wave_legacy_(0x02, length_, ch1_trg_);
			send_task_ = SEND_TASK::READY;
			break;

		case SEND_TASK::TRG:
			if(cap_cnt_ == 0 && !trg_enable_) ;
			else break;
			copy_(trg_pos_, 1024);
			send_wave_(0x01, 1024, ch0_trg_);
			send_wave_(0x02, 1024, ch1_trg_);
			send_task_ = SEND_TASK::READY;
			break;

		case SEND_TASK::BIN_SINGLE:
			copy_(cap_pos_, 1);
			send_wave_(0x01, 1, ch0_trg_);
			send_wave_(0x02, 1, ch1_trg_);
			send_task_ = SEND_TASK::READY;
			break;

		case SEND_TASK::BIN_MULTI:
			copy_(cap_pos_, 1024);
			send_wave_(0x01, 1024, ch0_trg_);
			send_wave_(0x02, 1024, ch1_trg_);
			send_task_ = SEND_TASK::READY;
			break;

		case SEND_TASK::ASCII_SINGLE:
			{
				copy_(cap_pos_, 1);
				char tmp[128];
				utils::sformat("%d,%d\r\n", tmp, sizeof(tmp)) % ch0_trg_[0] % ch1_trg_[0];
				m_sci_.puts(tmp);
			}
			send_task_ = SEND_TASK::READY;
			break;
		case SEND_TASK::ASCII_MULTI:
			{
				copy_(cap_pos_, 1024);
				for(uint16_t i = 0; i < 1024; ++i) {
					char tmp[128];
					utils::sformat("%d,%d,%d\r\n", tmp, sizeof(tmp))
						% i % ch0_trg_[i] % ch1_trg_[i];
					m_sci_.puts(tmp);
				}
			}
			send_task_ = SEND_TASK::READY;
			break;

		default:
			break;
		}
#if 1
		++cnt;
		if(cnt >= 50) {
			cnt = 0;
		}
		if(cnt >= 35) {
			LED::P = 1;
		} else {
			LED::P = 0;
		}
#endif
	}
}

