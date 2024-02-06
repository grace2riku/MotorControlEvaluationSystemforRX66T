#pragma once
//=====================================================================//
/*! @file
    @brief  電卓関数クラス
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2020, 2024 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RX/blob/master/LICENSE
*/
//=====================================================================//
#include <cmath>
#include <cstring>

namespace utils {

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
    /*!
        @brief  CALC FUNC クラス
		@param[in]	NVAL	数値クラス
    */
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template <class NVAL>
    class calc_func {
	public:
		//=============================================================//
		/*!
			@brief	関数 (0xC0 to 0xFF)、定義
		*/
		//=============================================================//
		enum class NAME : uint8_t {
			NONE = 0,		///< 未定義
			SIN = 0xC0,		///< sin(x)
			COS,			///< cos(x)
			TAN,			///< tan(x)
			ASIN,			///< asin(x)
			ACOS,			///< acos(x)
			ATAN,			///< atan(x)

			SQRT,			///< sqrt(x)
			LOG,			///< log(x)
			LN,				///< ln(x)

			EXP10,			///< exp10(x)
		};


		//=============================================================//
		/*!
			@brief  角度表現型
		*/
		//=============================================================//
		enum class ATYPE : uint8_t {
			Deg,	///< Deg 度数法 (360)
			Rad,	///< Rad 弧度法 (2pai)
			Grad	///< グラード (400)
		};


		//=============================================================//
		/*!
			@brief  数値表現型
		*/
		//=============================================================//
		enum class VTYPE : uint8_t {
			Dec,	///< １０進
			Hex,	///< １６進
			Bin		///< ２進
		};


	private:
		NVAL	pai_;
		NVAL	rad_;
		NVAL	grad_;
		NVAL	c2_;

		ATYPE	atype_;
		VTYPE	vtype_;

	public:
		//-------------------------------------------------------------//
		/*!
			@brief  コンストラクタ
		*/
		//-------------------------------------------------------------//
		calc_func() noexcept :
			rad_(360), grad_(400), c2_(2),
			atype_(ATYPE::Deg), vtype_(VTYPE::Dec)
		{ }


		//-------------------------------------------------------------//
		/*!
			@brief  角度からラジアンへの変換
			@param[in]	ang		角度（０～３６０）
			@return ラジアン（2 x pai)
		*/
		//-------------------------------------------------------------//
		NVAL conv_to_rad(const NVAL& in) noexcept
		{
			switch(atype_) {
			case ATYPE::Deg:
				return in / rad_ * c2_ * NVAL::get_pi();
			case ATYPE::Rad:
				return in;
			case ATYPE::Grad:
				return in / grad_ * c2_ * NVAL::get_pi();
			}
			return in;
		}


		NVAL conv_rad_to(const NVAL& in) noexcept
		{
			switch(atype_) {
			case ATYPE::Deg:
				return in / (c2_ * NVAL::get_pi()) * rad_; 
			case ATYPE::Rad:
				return in;
			case ATYPE::Grad:
				return in / (c2_ * NVAL::get_pi()) * grad_; 
			}
			return in;
		}


		//-------------------------------------------------------------//
		/*!
			@brief  角度型の指定
			@param[in]	t	角度型
		*/
		//-------------------------------------------------------------//
		void set_atype(ATYPE t) noexcept { atype_ = t; }


		//-------------------------------------------------------------//
		/*!
			@brief  角度型の取得
			@return	角度型
		*/
		//-------------------------------------------------------------//
		auto get_atype() const noexcept { return atype_; }


		//-------------------------------------------------------------//
		/*!
			@brief  数値型の指定
			@param[in]	t	数値型
		*/
		//-------------------------------------------------------------//
		void set_vtype(VTYPE t) noexcept { vtype_ = t; }


		//-------------------------------------------------------------//
		/*!
			@brief  角度型の取得
			@return	角度型
		*/
		//-------------------------------------------------------------//
		auto get_vtype() const noexcept { return vtype_; }



		//-------------------------------------------------------------//
		/*!
			@brief  関数文字列の取得
			@param[in]	name	関数名
			@return 関数文字列
		*/
		//-------------------------------------------------------------//
		const char* get_name(NAME name) const
		{
			switch(name) {
			case NAME::SIN:  return "sin";
			case NAME::COS:  return "cos";
			case NAME::TAN:  return "tan";
			case NAME::ASIN:  return "asin";
			case NAME::ACOS:  return "acos";
			case NAME::ATAN:  return "atan";
			case NAME::SQRT: return "sqrt";
			case NAME::LOG:  return "log";
			case NAME::LN:   return "ln";
			case NAME::EXP10: return "exp10";
			default:
				break;
			}
			return "";
		}


		//-------------------------------------------------------------//
		/*!
			@brief  関数名の取得
			@param[in]	text	関数文字列
			@param[out]	name	関数名型
			@return 移動後のポインター
		*/
		//-------------------------------------------------------------//
		const char* get_code(const char* text, NAME& name) const
		{
			if(text == nullptr) {
				name = NAME::NONE;
				return text;
			}

			if(strncmp(text, "sin", 3) == 0) { name = NAME::SIN; return text + 3; }
			else if(strncmp(text, "cos", 3) == 0) { name = NAME::COS; return text + 3; }
			else if(strncmp(text, "tan", 3) == 0) { name =  NAME::TAN; return text + 3; }
			else if(strncmp(text, "asin", 4) == 0) { name = NAME::ASIN; return text + 4; }
			else if(strncmp(text, "acos", 4) == 0) { name = NAME::ACOS; return text + 4; }
			else if(strncmp(text, "atan", 4) == 0) { name = NAME::ATAN; return text + 4; }
			else if(strncmp(text, "sqrt", 4) == 0) { name = NAME::SQRT; return text + 4; }
			else if(strncmp(text, "log", 3) == 0) { name = NAME::LOG; return text + 3; }
			else if(strncmp(text, "ln", 2) == 0) { name = NAME::LN; return text + 2; }
			else if(strncmp(text, "exp10", 5) == 0) { name = NAME::EXP10; return text + 5; }

			name = NAME::NONE;
			return text;
		}


		//-------------------------------------------------------------//
		/*!
			@brief  関数文字列の取得
			@param[in]	name	関数名
		*/
		//-------------------------------------------------------------//
		uint32_t get_name_size(NAME name) const
		{
			return strlen(get_name(name)) * 8;
		}


		//-------------------------------------------------------------//
		/*!
			@brief	  関数機能
			@param[in]	name	関数名
			@param[in]	in		入力
			@param[out]	out		出力
			@return 関数機能がマッチしない場合「false」
		*/
		//-------------------------------------------------------------//
		bool operator() (NAME name, const NVAL& in, NVAL& out) noexcept
		{
			switch(name) {
			case NAME::SIN:
				{
					auto a = conv_to_rad(in);
					out = NVAL::sin(a);
				}
				break;
			case NAME::COS:
				{
					auto a = conv_to_rad(in);
					out = NVAL::cos(a);
				}
				break;
			case NAME::TAN:
				{
					auto a = conv_to_rad(in);
					out = NVAL::tan(a);
				}
				break;
			case NAME::ASIN:
				{
					auto a = NVAL::asin(in);
					out = conv_rad_to(a);
				}
				break;
			case NAME::ACOS:
				{
					auto a = NVAL::acos(in);
					out = conv_rad_to(a);
				}
				break;
			case NAME::ATAN:
				{
					auto a = NVAL::atan(in);
					out = conv_rad_to(a);
				}
				break;



			case NAME::SQRT:
				{
					out = NVAL::sqrt(in);
				}
				break;
			case NAME::LOG:
				{
					out = NVAL::log10(in);
				}
				break;
			case NAME::LN:
				{
					out = NVAL::log(in);
				}
				break;

			case NAME::EXP10:
				{
					out = NVAL::exp10(in);
				}
				break;
			default:
				return false;
			}
			return true;
		}
	};
}
