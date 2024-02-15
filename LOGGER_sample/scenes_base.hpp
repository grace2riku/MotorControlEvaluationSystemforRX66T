#pragma once
//=====================================================================//
/*!	@file
	@brief	シーン共通関係
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2018, 2022 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RX/blob/master/LICENSE
*/
//=====================================================================//
#include "graphics/font8x16.hpp"
#include "graphics/kfont.hpp"
#include "graphics/font.hpp"
#include "graphics/graphics.hpp"
#include "graphics/picojpeg_in.hpp"
#include "graphics/scaling.hpp"
#include "graphics/img_in.hpp"
#include "graphics/root_menu.hpp"
#include "graphics/simple_dialog.hpp"
#include "graphics/simple_filer.hpp"
#include "chip/FT5206.hpp"

#include "common/spi_io2.hpp"

#include "gui/widget_director.hpp"

#define USE_SDHI

// #define SOFT_I2C

#ifdef SOFT_I2C
#include "common/si2c_io.hpp"
#else
#include "common/sci_i2c_io.hpp"
#endif

#include "common/cmt_mgr.hpp"
#include "common/nmea_dec.hpp"

#include "resource.hpp"

namespace app {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	シーン ID
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	enum class scene_id {
		title,
		root_menu,

		laptime,
		recall,
		setup,
		gps
	};


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	シーン・ベース
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	class scenes_base {

		/// GLCDC
		static constexpr int16_t LCD_X = 480;
		static constexpr int16_t LCD_Y = 272;
		static constexpr auto PIX = graphics::pixel::TYPE::RGB565;
        typedef utils::fixed_fifo<uint8_t, 64> RB64;
        typedef utils::fixed_fifo<uint8_t, 64> SB64;
#if defined(SIG_RX65N)
		typedef device::PORT<device::PORT0, device::bitpos::B5> SW2;

		typedef device::PORT<device::PORT6, device::bitpos::B3> LCD_DISP;
		typedef device::PORT<device::PORT6, device::bitpos::B6> LCD_LIGHT;
		static constexpr uint32_t LCD_ORG = 0x0000'0100;
		typedef device::PORT<device::PORT0, device::bitpos::B7> FT5206_RESET;
        typedef device::sci_i2c_io<device::SCI6, RB64, SB64, device::port_map::ORDER::FIRST> FT5206_I2C;
#elif defined(SIG_RX72N)
		typedef device::PORT<device::PORT0, device::bitpos::B7> SW2;

        typedef device::PORT<device::PORTB, device::bitpos::B3> LCD_DISP;
        typedef device::PORT<device::PORT6, device::bitpos::B7> LCD_LIGHT;
        static constexpr uint32_t LCD_ORG = 0x0080'0000;
		typedef device::PORT<device::PORT6, device::bitpos::B6> FT5206_RESET;
        typedef device::sci_i2c_io<device::SCI6, RB64, SB64, device::port_map::ORDER::SECOND> FT5206_I2C;
#endif
		typedef device::glcdc_mgr<device::GLCDC, LCD_X, LCD_Y, PIX> GLCDC_MGR;

	public:
		typedef graphics::font8x16 AFONT;
#ifdef CASH_KFONT
		typedef graphics::kfont<16, 16, 64> KFONT;
#else
		typedef graphics::kfont<16, 16> KFONT;
#endif
		typedef graphics::font<AFONT, KFONT> FONT;

		typedef device::drw2d_mgr<GLCDC_MGR, FONT> RENDER;
//		typedef graphics::render<GLCDC_MGR, FONT> RENDER;

		// FT5206 touch device
		typedef chip::FT5206<FT5206_I2C> TOUCH;

		typedef resource<RENDER> RESOURCE;

		typedef gui::simple_dialog<RENDER, TOUCH> DIALOG;

		typedef gui::simple_filer_base FILER_BASE;
		typedef gui::simple_filer<RENDER> FILER;

		typedef gui::widget_director<RENDER, TOUCH, 32> WIDD;

	private:
		GLCDC_MGR	glcdc_mgr_;
		AFONT		afont_;
		KFONT		kfont_;
		FONT		font_;
		RENDER		render_;

	public:
		// メニューボタンの描画
		class BACK {
			RENDER&	render_;
		public:
			BACK(RENDER& render) : render_(render) { }

			void operator () (const vtx::srect& rect)
			{
				render_.round_box(rect, 8);
				render_.swap_color();
				auto r = rect;
				r.org += 2;
				r.size -= 2 * 2;
				render_.round_box(r, 8 - 2);
			}
		};
		typedef gui::root_menu<RENDER, BACK, 8> MENU;

		typedef img::scaling<RENDER> PLOT;
		typedef img::img_in<PLOT> IMG_IN;

		// CMT 1/100 秒計測
		class watch_task {
		public:
			static const uint32_t LAP_LIMIT = 500;

		private:
			uint32_t	count_;
			bool		enable_;
			bool		lvl_;

			uint32_t	lap_pos_;
			uint32_t	lap_pad_[LAP_LIMIT];

		public:
			watch_task() : count_(0), enable_(false), lvl_(false),
				lap_pos_(0), lap_pad_{ 0 } { }

			void operator() () noexcept
			{
				bool lvl = !SW2::P();
				if(enable_) {
					if(!lvl_ && lvl) {
						if(lap_pos_ < LAP_LIMIT) {
							lap_pad_[lap_pos_] = count_;
							++lap_pos_;
						}
					}
					++count_;
				}
				lvl_ = lvl;
			}

			void enable(bool ena = true) noexcept { enable_ = ena; }

			void set(uint32_t count) noexcept { count_ = count; }

			uint32_t get() const noexcept { return count_; }

			void reset() noexcept
			{
				count_ = 0;
				lap_pos_ = 0;
			}

			uint32_t get_lap_pos() const noexcept { return lap_pos_; }

			uint32_t get_lap(uint32_t pos) const noexcept {
				if(pos >= LAP_LIMIT) return 0;
				return lap_pad_[pos];
			}
		};

		typedef device::cmt_mgr<device::CMT1, watch_task> CMT;

		// GPS 専用シリアル定義
		// RX72N Envision Kit: Pmod1: (3)P52:RXD2, (2)P50:TXD2, (5)GND
		typedef utils::fixed_fifo<char, 2048>  G_REB;
		typedef utils::fixed_fifo<char, 512> G_SEB;
		typedef device::sci_io<device::SCI2, G_REB, G_SEB, device::port_map::ORDER::SECOND> GPS_S;

		typedef utils::nmea_dec<GPS_S> NMEA;


	private:
		FT5206_I2C	ft5206_i2c_;
		TOUCH		touch_;

		WIDD		widd_;
		MENU		menu_;
		BACK		back_;
		DIALOG		dialog_;

		FILER		filer_;

		CMT			cmt_;

		RESOURCE	resource_;

		PLOT		plot_;
		IMG_IN		img_in_;

		GPS_S		gps_s_;
		NMEA		nmea_;

		bool		enable_filer_;

	public:
		//-------------------------------------------------------------//
		/*!
			@brief	コンストラクタ
		*/
		//-------------------------------------------------------------//
		scenes_base() noexcept :
			glcdc_mgr_(nullptr, reinterpret_cast<void*>(LCD_ORG)),
			afont_(), kfont_(), font_(afont_, kfont_),
			render_(glcdc_mgr_, font_),
			touch_(ft5206_i2c_),
			widd_(render_, touch_),
			menu_(render_, back_), back_(render_), dialog_(render_, touch_),
			filer_(render_),
			resource_(render_),
			plot_(render_), img_in_(plot_),
			gps_s_(), nmea_(gps_s_),
			enable_filer_(false)
			{ }


		//-------------------------------------------------------------//
		/*!
			@brief	ベースの初期化
		*/
		//-------------------------------------------------------------//
		void init() noexcept
		{
			{
				auto intr = device::ICU::LEVEL::_1;
				nmea_.start(intr);
			}

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

			{  // レンダラー初期化
				auto ver = render_.get_version();
				utils::format("DRW2D Version: %04X\n") % ver;
				if(render_.start(device::ICU::LEVEL::_2)) {
					utils::format("Start DRW2D\n");
				} else {
					utils::format("DRW2D Fail\n");
				}
			}

			{  // FT5206 touch screen controller
				TOUCH::reset<FT5206_RESET>();
				auto intr_lvl = device::ICU::LEVEL::_1;
				if(!ft5206_i2c_.start(FT5206_I2C::MODE::MASTER, FT5206_I2C::SPEED::STANDARD, intr_lvl)) {
					utils::format("FT5206 I2C Start Fail...\n");
				}
				if(!touch_.start()) {
					utils::format("FT5206 Start Fail...\n");
				}
			}

			{  // CMT 100Hz タイマー
				auto intr_level = device::ICU::LEVEL::_5;
				cmt_.start(100, intr_level);
			}

			// スイッチ入力
			SW2::DIR = 0;

			// タッチパネルの初期化準備
			dialog_.ready_to_touch();

			
		}


		//-------------------------------------------------------------//
		/*!
			@brief	同期と、タッチパネルデータ更新
		*/
		//-------------------------------------------------------------//
		void sync() noexcept
		{
			render_.sync_frame();
			touch_.update();
		}


		//-------------------------------------------------------------//
		/*!
			@brief	アップデート（毎フレーム呼び出し）
		*/
		//-------------------------------------------------------------//
		void update() noexcept
		{
			nmea_.service();
//			nmea_.list_all();

			// ファイラー機能更新
			if(enable_filer_) {
				uint32_t ctrl = 0;
#if 0
				auto data = get_fami_pad();
				if(chip::on(data, chip::FAMIPAD_ST::SELECT)) {
					gui::set(gui::filer_ctrl::OPEN, ctrl);
				}
				if(chip::on(data, chip::FAMIPAD_ST::UP)) {
					gui::set(gui::filer_ctrl::UP, ctrl);
				}
				if(chip::on(data, chip::FAMIPAD_ST::DOWN)) {
					gui::set(gui::filer_ctrl::DOWN, ctrl);
				}
				if(chip::on(data, chip::FAMIPAD_ST::LEFT)) {
					gui::set(gui::filer_ctrl::BACK, ctrl);
				}
				if(chip::on(data, chip::FAMIPAD_ST::RIGHT)) {
					gui::set(gui::filer_ctrl::SELECT, ctrl);
				}
#endif
				auto tnum = touch_.get_touch_num();
				const auto& xy = touch_.get_touch_pos(0);
				filer_.set_touch(tnum, xy.pos);
				char path[256];
				auto fst = filer_.update(ctrl, path, sizeof(path));
				if(fst == FILER_BASE::status::FILE) {
					render_.sync_frame();

					utils::format("%s\n") % path;

					enable_filer_ = false;
				}
			}

			// widget 関係
			widd_.update();
		}


		//-------------------------------------------------------------//
		/*!
			@brief	GLCDC_MGR の参照
			@return GLCDC_MGR
		*/
		//-------------------------------------------------------------//
		GLCDC_MGR& at_glcdc_mgr() noexcept { return glcdc_mgr_; }


		//-------------------------------------------------------------//
		/*!
			@brief	タッチデバイスの参照
			@return タッチデバイス
		*/
		//-------------------------------------------------------------//
		TOUCH& at_touch() noexcept { return touch_; }


		//-------------------------------------------------------------//
		/*!
			@brief	RENDER の参照
			@return RENDER
		*/
		//-------------------------------------------------------------//
		RENDER& at_render() noexcept { return render_; }


		//-------------------------------------------------------------//
		/*!
			@brief	widget_director の参照
			@return widget_director
		*/
		//-------------------------------------------------------------//
		auto& at_widget_director() noexcept { return widd_; }


		//-------------------------------------------------------------//
		/*!
			@brief	MENU の参照
			@return MENU
		*/
		//-------------------------------------------------------------//
		MENU& at_menu() noexcept { return menu_; }


		//-------------------------------------------------------------//
		/*!
			@brief	DIALOG の参照
			@return DIALOG
		*/
		//-------------------------------------------------------------//
		DIALOG& at_dialog() noexcept { return dialog_; }


		//-------------------------------------------------------------//
		/*!
			@brief	CMT の参照
			@return CMT
		*/
		//-------------------------------------------------------------//
		CMT& at_cmt() noexcept { return cmt_; }


		//-------------------------------------------------------------//
		/*!
			@brief	RESOURCE の参照
			@return RESOURCE
		*/
		//-------------------------------------------------------------//
		RESOURCE& at_resource() noexcept { return resource_; }


		//-------------------------------------------------------------//
		/*!
			@brief	PLOT の参照
			@return PLOT
		*/
		//-------------------------------------------------------------//
		PLOT& at_plot() noexcept { return plot_; }


		//-------------------------------------------------------------//
		/*!
			@brief	IMG_IN の参照
			@return IMG_IN
		*/
		//-------------------------------------------------------------//
		IMG_IN& at_img() noexcept { return img_in_; }


		//-------------------------------------------------------------//
		/*!
			@brief	NMEA の参照
			@return NMEA
		*/
		//-------------------------------------------------------------//
		NMEA& at_nmea() noexcept { return nmea_; }


		//-------------------------------------------------------------//
		/*!
			@brief	ファイラーの許可
			@param[in]	ena		不許可の場合「false」
		*/
		//-------------------------------------------------------------//
		void enable_filer(bool ena = true) noexcept { enable_filer_ = ena; }


		//-------------------------------------------------------------//
		/*!
			@brief	ファイラーの状態を取得
			@return ファイラーの状態
		*/
		//-------------------------------------------------------------//
		bool get_filer_state() const noexcept { return enable_filer_; }
	};
}

extern void change_scene(app::scene_id id);
extern app::scenes_base& at_scenes_base();
