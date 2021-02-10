#pragma once
//=====================================================================//
/*!	@file
	@brief	Tiny 3D Glaphics Library (Tiny OpenGL)
    @author 平松邦仁 (hira@rvf-rc45.net)
	@copyright	Copyright (C) 2018, 2021 Kunihito Hiramatsu @n
				Released under the MIT license @n
				https://github.com/hirakuni45/RX/blob/master/LICENSE
*/
//=====================================================================//
#include "common/vtx.hpp"
#include "graphics/color.hpp"
#include "graphics/glmatrix.hpp"
#include "graphics/tgl_base.hpp"

namespace graphics {

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	/*!
		@brief	TinyGL class
		@param[in]	RDR		レンダークラス（DRW2D インスタンス）
		@param[in]	VNUM	最大頂点数
		@param[in]	PNUM	最大プリミティブ数
	*/
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	template<class RDR, uint32_t VNUM, uint32_t PNUM>
	class tgl : public tgl_base {

		RDR&		rdr_;

		struct vtx_t {
			vtx::fvtx	p_;
			vtx::fvtx	n_;
			bool		normal_;
			vtx_t() : p_(), n_(), normal_(false) { }
		};

		uint32_t	vtx_idx_;
		vtx_t		vtxs_[VNUM];

		struct dt_t {
			PTYPE		pt_;
			share_color	col_;
			uint32_t	org_;
			uint32_t	end_;
			dt_t() : pt_(PTYPE::NONE), col_(0, 0, 0), org_(0), end_(0) { }
		};

		uint32_t	dt_idx_;
		dt_t		dts_[PNUM];

		share_color	color_;

		MATRIX		matrix_;

		bool test_cw_(const vtx::spos* p, int32_t num) noexcept
		{
			for(int32_t i = 0; i < (num - 1); ++i) {
				auto n = i + 1;
				if(p[i].y >= p[n].y) {
					if(p[i].x < p[n].x) return true;  // CW
					else if(p[i].x > p[n].x) return false;  // CCW
				} else {
					if(p[i].x > p[n].x) return true;  // CW
					else if(p[i].x < p[n].x) return false;  // CCW
				}
			}
			// 判定が出来ない場合は「false」
			return false;
		}

	public:
		//-----------------------------------------------------------------//
		/*!
			@brief	コンストラクター
			@param[in]	rdr		レンダークラス（DRW2D インスタンス）
		*/
		//-----------------------------------------------------------------//
		tgl(RDR& rdr) noexcept : rdr_(rdr),
			vtx_idx_(0), vtxs_{},
			dt_idx_(0), dts_{},
			color_(0, 0, 0),
			matrix_()
		{ }


		//-----------------------------------------------------------------//
		/*!
			@brief	開始
			@return 成功なら「true」
		*/
		//-----------------------------------------------------------------//
		bool start() noexcept
		{
#ifdef SIG_RX72N
			// GNU-RX 8.3.0 compile options: -mtfu=intrinsic,mathlib
			// see: common/mtx.hpp
			utils::format("TinyGL Start\n");
			__init_tfu();
			utils::format("  TFU initializations\n");
#endif
//	void __builtin_rx_atan2hypotf(float, float, float*, float*);
//	float __builtin_rx_sinf(float);
//	float __builtin_rx_cosf(float);
//	float __builtin_rx_atan2f(float, float);
//	float __builtin_rx_hypotf(float, float);
			return true;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	描画線サイズ指定
			@param[in]	w	線幅
		*/
		//-----------------------------------------------------------------//
		void LineWidth(float w) noexcept
		{
			rdr_.set_pen_size(static_cast<int16_t>(w * 16.0f));
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	描画開始
			@param[in]	pt	描画タイプ
		*/
		//-----------------------------------------------------------------//
		void Begin(PTYPE pt) noexcept
		{
			if(dt_idx_ >= PNUM) return;

			dts_[dt_idx_].pt_ = pt;
			dts_[dt_idx_].col_ = color_;
			dts_[dt_idx_].org_ = vtx_idx_;
			dts_[dt_idx_].end_ = vtx_idx_;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	描画終了
		*/
		//-----------------------------------------------------------------//
		void End() noexcept
		{
			if(dts_[dt_idx_].org_ == vtx_idx_) {
				return;
			}
			dts_[dt_idx_].end_ = vtx_idx_;
			++dt_idx_;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	色設定
			@param[in]	c	カラー
		*/
		//-----------------------------------------------------------------//
		void Color(const share_color& c) noexcept
		{
			color_ = c;
		}


		//-----------------------------------------------------------------//
		/*!
			@brief	2D 頂点の登録
			@param[in]	v	頂点
		*/
		//-----------------------------------------------------------------//
		void Vertex(float x, float y) noexcept
		{
			if(vtx_idx_ >= VNUM) return;
			auto& p = vtxs_[vtx_idx_].p_;
			p.x = x;
			p.y = y;
			p.z = 0.0f;
			++vtx_idx_;
		}
		void Vertex(const vtx::spos& v) noexcept { Vertex(v.x, v.y); }
		void Vertex(const vtx::ipos& v) noexcept { Vertex(v.x, v.y); }
		void Vertex(const vtx::fpos& v) noexcept { Vertex(v.x, v.y); }


		//-----------------------------------------------------------------//
		/*!
			@brief	3D 頂点の登録
			@param[in]	v	頂点
		*/
		//-----------------------------------------------------------------//
		void Vertex(float x, float y, float z) noexcept
		{
			if(vtx_idx_ >= VNUM) return;
			auto& p = vtxs_[vtx_idx_].p_;
			p.x = x;
			p.y = y;
			p.z = z;
			++vtx_idx_;
		}
		void Vertex(const vtx::svtx& v) noexcept { Vertex(v.x, v.y, v.z); }
		void Vertex(const vtx::ivtx& v) noexcept { Vertex(v.x, v.y, v.z); }
		void Vertex(const vtx::fvtx& v) noexcept { Vertex(v.x, v.y, v.z); }


		//-----------------------------------------------------------------//
		/*!
			@brief	法線ベクトルの登録
			@param[in]	n	法線ベクトル
		*/
		//-----------------------------------------------------------------//
		void Normal(float x, float y, float z) noexcept
		{
			if(vtx_idx_ >= VNUM) return;
			auto& n = vtxs_[vtx_idx_].n_;
			n.x = x;
			n.y = y;
			n.z = z;
			vtxs_[vtx_idx_].normal_ = true;
		}
		void Normal(const vtx::fvtx& v) noexcept { Normal(v.x, v.y, v.z); }


		//-----------------------------------------------------------------//
		/*!
			@brief	マトリックスへの参照
			@return マトリックス
		*/
		//-----------------------------------------------------------------//
		MATRIX& at_matrix() noexcept { return matrix_; }		


		//-----------------------------------------------------------------//
		/*!
			@brief	レンダリング
		*/
		//-----------------------------------------------------------------//
		void renderring() noexcept
		{
			MATRIX::matrix_type wm;
			matrix_.world_matrix(wm);

			int ox;
			int oy;
			int w;
			int h;
			matrix_.get_viewport(ox, oy, w, h);  // drw2d for fixed point
			ox <<= 4;
			oy <<= 4;
			w <<= 4;
			h <<= 4;

			for(uint32_t i = 0; i < dt_idx_; ++i) {
				const auto& t = dts_[i];
				rdr_.set_fore_color(t.col_);

				uint32_t k = 0;
				vtx::spos tmp[16];
				for(uint32_t j = t.org_; j < t.end_; ++j) {
					vtx::fvtx dst;
					matrix_.vertex_screen(wm, vtxs_[j].p_, dst);
					tmp[k].x = static_cast<int16_t>(dst.x * w) + (w / 2) + ox;
					tmp[k].y = static_cast<int16_t>(dst.y * h) + (h / 2) + oy;
					++k;
				}
				switch(t.pt_) {
				case PTYPE::POINTS:

					break;
				case PTYPE::LINES:
					for(uint32_t j = 0; j < k; j += 2) {
						rdr_.line_d(tmp[j], tmp[j+1]);
					}
					break;
				case PTYPE::LINE_STRIP:
					for(uint32_t j = 0; j < (k - 1); ++j) {
						rdr_.line_d(tmp[j], tmp[j+1]);
					}
					break;
				case PTYPE::LINE_LOOP:
					// if(!test_cw_(tmp, k)) break;  // ccw を描画しない
					for(uint32_t j = 0; j < k; ++j) {
						auto n = j + 1;
						if(n >= k) n = 0;
						rdr_.line_d(tmp[j], tmp[n]);
					}
					break;
				case PTYPE::QUAD:
					for(uint32_t j = 0; j < k; j += 4) {
						rdr_.quad_d(tmp[j+0], tmp[j+1], tmp[j+2], tmp[j+3]);
					}
					break;
				case PTYPE::TRIANGLE:
					for(uint32_t j = 0; j < k; j += 3) {
						rdr_.triangle_d(tmp[j+0], tmp[j+1], tmp[j+2]);
					}
					break;
				default:
					break;
				}
			}

			dt_idx_ = 0;
			vtx_idx_ = 0;
		}
	};
}
