//////////////////////////////////////////////////////////////////////////////////////
//
// (C) Daniel Strano 2017. All rights reserved.
//
// This is a SIMD implementation of the double precision complex type.
// The API is designed to (almost entirely) mirror that of the C++ standard library
// double precision complex type.
//
// Licensed under the GNU Lesser General Public License V3.
// See LICENSE.md in the project root or https://www.gnu.org/licenses/lgpl-3.0.en.html
// for details.

#include <emmintrin.h>

namespace Qrack {

	/// SIMD implementation of the double precision complex type
	/** SIMD implementation of the double precision complex type. */
	struct ComplexSimd {
		__m128d _val;

		ComplexSimd() {
		}
		
		ComplexSimd(__m128d v) {
			_val = v;
		}
		ComplexSimd(double real, double imag) {
			_val = _mm_set_pd(imag, real);
		}

		ComplexSimd operator+(const ComplexSimd& other) const {
			return ComplexSimd(_mm_add_pd(_val, other._val));
		}
		ComplexSimd operator+=(const ComplexSimd& other) {
			_val = _mm_add_pd(_val, other._val);
			return ComplexSimd(_val);
		}
		ComplexSimd operator-(const ComplexSimd& other) const {
			return ComplexSimd(_mm_sub_pd(_val, other._val));
		}
		ComplexSimd operator-=(const ComplexSimd& other) {
			_val = _mm_sub_pd(_val, other._val);
			return ComplexSimd(_val);
		}
		ComplexSimd operator*(const ComplexSimd& other) const {
			__v2df temp = (__v2df)_mm_mul_pd(other._val, _val);
			double real = temp[0] - temp[1];
			temp  = (__v2df)_mm_mul_pd(other._val,
				_mm_move_sd(
					_mm_castsi128_pd(_mm_bslli_si128(_mm_castpd_si128(_val), 8)),
					_mm_castsi128_pd(_mm_bsrli_si128(_mm_castpd_si128(_val), 8))
				)
			);			
			return ComplexSimd(real, temp[0] + temp[1]);
		}
		ComplexSimd operator*=(const ComplexSimd& other) {
			__v2df temp = (__v2df)_mm_mul_pd(other._val, _val);
			double real = temp[0] - temp[1];
			temp  = (__v2df)_mm_mul_pd(other._val,
				_mm_move_sd(
					_mm_castsi128_pd(_mm_bslli_si128(_mm_castpd_si128(_val), 8)),
					_mm_castsi128_pd(_mm_bsrli_si128(_mm_castpd_si128(_val), 8))
				)
			);	
			_val = _mm_set_pd(temp[0] + temp[1], real);
			return ComplexSimd(_val);
		}
		ComplexSimd operator*(const double rhs) const {
			return _mm_mul_pd(_val, _mm_set1_pd(rhs));
		}
		ComplexSimd operator/(const ComplexSimd& other) const {
			__v2df temp = (__v2df)_mm_mul_pd(other._val, other._val);
			double denom = temp[0] + temp[1];
			temp = (__v2df)_mm_mul_pd(other._val, _val);
			double realNumer = temp[0] + temp[1];
			temp = (__v2df)_mm_mul_pd(other._val,
				_mm_move_sd(
					_mm_castsi128_pd(_mm_bslli_si128(_mm_castpd_si128(_val), 8)),
					_mm_castsi128_pd(_mm_bsrli_si128(_mm_castpd_si128(_val), 8))
				)
			);	
			return ComplexSimd(_mm_div_pd(_mm_set_pd(temp[0] + temp[1], realNumer), _mm_set1_pd(denom)));
		}
		ComplexSimd operator/=(const ComplexSimd& other) {
			__v2df temp = (__v2df)_mm_mul_pd(other._val, other._val);
			double denom = temp[0] + temp[1];
			temp = (__v2df)_mm_mul_pd(other._val, _val);
			double realNumer = temp[0] + temp[1];
			temp = (__v2df)_mm_mul_pd(other._val,
				_mm_move_sd(
					_mm_castsi128_pd(_mm_bslli_si128(_mm_castpd_si128(_val), 8)),
					_mm_castsi128_pd(_mm_bsrli_si128(_mm_castpd_si128(_val), 8))
				)
			);	
			_val = _mm_div_pd(_mm_set_pd(temp[0] + temp[1], realNumer), _mm_set1_pd(denom));
			return ComplexSimd(_val);
		}
		ComplexSimd operator/(const double rhs) const {
			return _mm_div_pd(_val, _mm_set1_pd(rhs));
		}
		ComplexSimd operator/=(const double rhs) {
			_val = _mm_div_pd(_val, _mm_set1_pd(rhs));
			return ComplexSimd(_val);
		}
	};

	static ComplexSimd operator*(const double lhs, const ComplexSimd& rhs) {
		return _mm_mul_pd(_mm_set1_pd(lhs), rhs._val);
	}
	static ComplexSimd operator/(const double lhs, const ComplexSimd& rhs) {
		__v2df temp = (__v2df)_mm_mul_pd(rhs._val, rhs._val);
		double denom = temp[0] + temp[1];
		temp = (__v2df)_mm_div_pd(_mm_mul_pd(rhs._val, _mm_set1_pd(lhs)), _mm_set1_pd(denom));	
		return ComplexSimd(temp[0], -temp[1]);
	}
	double real(const ComplexSimd& cmplx) {
		return ((__v2df)(cmplx._val))[0];
	}
	double imag(const ComplexSimd& cmplx) {
		return ((__v2df)(cmplx._val))[1];
	}
	double normSqrd(const ComplexSimd& cmplx) {
		__v2df temp = (__v2df)_mm_mul_pd(cmplx._val, cmplx._val);
		return (temp[0] + temp[1]); 
	}
}
