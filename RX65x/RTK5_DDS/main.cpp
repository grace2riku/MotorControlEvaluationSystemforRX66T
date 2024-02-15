//=====================================================================//
/*! @file
    @brief  RX65N ダイレクト・デジタル・シンセサイザ @n
			マイコン内臓１２ビットＤ／Ａ変換を使って、波形を生成するガジェット
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2019 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RX/blob/master/LICENSE
*/
//=====================================================================//
#define CASH_KFONT

#include "common/renesas.hpp"
#include "common/cmt_mgr.hpp"
#include "common/fixed_fifo.hpp"
#include "common/sci_io.hpp"
#include "common/sci_i2c_io.hpp"
#include "common/format.hpp"
#include "common/command.hpp"
#include "common/shell.hpp"
#include "common/spi_io2.hpp"
#include "common/tpu_io.hpp"
#include "graphics/font8x16.hpp"
#include "graphics/graphics.hpp"
#include "graphics/simple_filer.hpp"
#include "graphics/kfont.hpp"
#include "graphics/font.hpp"

#include "chip/FT5206.hpp"

#define USE_SDHI

namespace {

	typedef device::PORT<device::PORT7, device::bitpos::B0> LED;

	typedef device::cmt_mgr<device::CMT0, utils::null_task> CMT;
	CMT			cmt_;

	typedef utils::fixed_fifo<char, 512>  RECV_BUFF;
	typedef utils::fixed_fifo<char, 1024> SEND_BUFF;
	typedef device::sci_io<device::SCI9, RECV_BUFF, SEND_BUFF> SCI;
	SCI			sci_;

	// カード電源制御は使わないので、「device::NULL_PORT」を指定する。
//	typedef device::PORT<device::PORT6, device::bitpos::B4> SDC_POWER;
	typedef device::NULL_PORT SDC_POWER;

	// 書き込み禁止は使わない
	typedef device::NULL_PORT SDC_WPRT;

#ifdef USE_SDHI
	// RX65N Envision Kit の SDHI ポートは、候補３になっている
	typedef fatfs::sdhi_io<device::SDHI, SDC_POWER, SDC_WPRT,
		device::port_map::ORDER::THIRD> SDHI;
	SDHI		sdh_;
#else
	// Soft SDC 用　SPI 定義（SPI）
	typedef device::PORT<device::PORT2, device::bitpos::B2> MISO;  // DAT0
	typedef device::PORT<device::PORT2, device::bitpos::B0> MOSI;  // CMD
	typedef device::PORT<device::PORT2, device::bitpos::B1> SPCK;  // CLK

	typedef device::spi_io2<MISO, MOSI, SPCK> SPI;  ///< Soft SPI 定義

	SPI			spi_;

	typedef device::PORT<device::PORT1, device::bitpos::B7> SDC_SELECT;  // DAT3 カード選択信号
	typedef device::PORT<device::PORT2, device::bitpos::B5> SDC_DETECT;  // CD   カード検出

	typedef fatfs::mmc_io<SPI, SDC_SELECT, SDC_POWER, SDC_DETECT> MMC;   // ハードウェアー定義

	MMC			sdh_(spi_, 20000000);
#endif

	typedef device::PORT<device::PORT6, device::bitpos::B3> LCD_DISP;
	typedef device::PORT<device::PORT6, device::bitpos::B6> LCD_LIGHT;
	static const int16_t LCD_X = 480;
	static const int16_t LCD_Y = 272;
	static void* LCD_ORG = reinterpret_cast<void*>(0x00000100);
	static const auto PIXT = graphics::pixel::TYPE::RGB565;
	typedef device::glcdc_mgr<device::GLCDC, LCD_X, LCD_Y, PIXT> GLCDC_MGR;
	GLCDC_MGR	glcdc_mgr_(nullptr, LCD_ORG);

	typedef graphics::font8x16 AFONT;
	AFONT		afont_;
#ifdef CASH_KFONT
	typedef graphics::kfont<16, 16, 64> KFONT;
#else
	typedef graphics::kfont<16, 16> KFONT;
#endif
	KFONT		kfont_;
	typedef graphics::font<AFONT, KFONT> FONT;
	FONT		font_(afont_, kfont_);

//	typedef device::drw2d_mgr<GLCDC_MGR, FONT> RENDER;
	typedef graphics::render<GLCDC_MGR, FONT> RENDER;
	RENDER		render_(glcdc_mgr_, font_);

	// FT5206, SCI6 簡易 I2C 定義
	typedef device::PORT<device::PORT0, device::bitpos::B7> FT5206_RESET;
	typedef utils::fixed_fifo<uint8_t, 64> RB6;
	typedef utils::fixed_fifo<uint8_t, 64> SB6;
	typedef device::sci_i2c_io<device::SCI6, RB6, SB6,
		device::port_map::ORDER::FIRST> FT5206_I2C;

	FT5206_I2C	ft5206_i2c_;
	typedef chip::FT5206<FT5206_I2C> FT5206;
	FT5206		ft5206_(ft5206_i2c_);

	typedef utils::command<256> CMD;
	CMD			cmd_;
	typedef utils::shell<CMD> SHELL;
	SHELL		shell_(cmd_);

	void update_led_()
	{
		static uint8_t n = 0;
		++n;
		if(n >= 30) {
			n = 0;
		}
		if(n < 10) {
			LED::P = 0;
		} else {
			LED::P = 1;
		}
	}


	void command_()
	{
		if(!cmd_.service()) {
			return;
		}
		if(shell_.analize()) {
			return;
		}
		if(cmd_.cmp_word(0, "help")) {
			shell_.help();
		} else {
			utils::format("Command error: '%s'\n") % cmd_.get_command();
		}
	}
}


extern "C" {

	void sci_putch(char ch)
	{
		sci_.putch(ch);
	}


	void sci_puts(const char* str)
	{
		sci_.puts(str);
	}


	char sci_getch(void)
	{
		return sci_.getch();
	}


	uint16_t sci_length()
	{
		return sci_.recv_length();
	}


	DSTATUS disk_initialize(BYTE drv) {
		return sdh_.disk_initialize(drv);
	}


	DSTATUS disk_status(BYTE drv) {
		return sdh_.disk_status(drv);
	}


	DRESULT disk_read(BYTE drv, BYTE* buff, DWORD sector, UINT count) {
		return sdh_.disk_read(drv, buff, sector, count);
	}


	DRESULT disk_write(BYTE drv, const BYTE* buff, DWORD sector, UINT count) {
		return sdh_.disk_write(drv, buff, sector, count);
	}


	DRESULT disk_ioctl(BYTE drv, BYTE ctrl, void* buff) {
		return sdh_.disk_ioctl(drv, ctrl, buff);
	}


	DWORD get_fattime(void) {
		time_t t = 0;
///		rtc_.get_time(t);
		return utils::str::get_fattime(t);
	}
}

int main(int argc, char** argv);

int main(int argc, char** argv)
{
	SYSTEM_IO::boost_master_clock();

	{  // SCI 設定
		auto sci_level = device::ICU::LEVEL::_2;
		sci_.start(115200, sci_level);
	}

	{  // SD カード・クラスの初期化
		sdh_.start();
	}

	utils::format("RTK5RX65N Start for Direct Digital Synthsyzer\n");
	cmd_.set_prompt("# ");

	{  // GLCDC の初期化
		LCD_DISP::DIR  = 1;
		LCD_LIGHT::DIR = 1;
		LCD_DISP::P  = 0;  // DISP Disable
		LCD_LIGHT::P = 0;  // BackLight Disable (No PWM)
		if(glcdc_mgr_.start(device::ICU::LEVEL::_2)) {
			utils::format("Start GLCDC\n");
			LCD_DISP::P  = 1;  // DISP Enable
			LCD_LIGHT::P = 1;  // BackLight Enable (No PWM)
			if(!glcdc_mgr_.control(GLCDC_MGR::CONTROL_CMD::START_DISPLAY)) {
				utils::format("GLCDC ctrl fail...\n");
			}
		} else {
			utils::format("GLCDC Fail\n");
		}
	}

	{  // FT5206 touch screen controller
		FT5206::reset<FT5206_RESET>();
		auto intr_lvl = device::ICU::LEVEL::_1;
		if(!ft5206_i2c_.start(FT5206_I2C::MODE::MASTER, FT5206_I2C::SPEED::STANDARD, intr_lvl)) {
			utils::format("FT5206 I2C Start Fail...\n");
		}
		if(!ft5206_.start()) {
			utils::format("FT5206 Start Fail...\n");
		}
	}

	LED::DIR = 1;

	// タッチパネルの安定待ち



	while(1) {
		render_.sync_frame();

		ft5206_.update();

		sdh_.service();

		command_();

		update_led_();
	}
}
