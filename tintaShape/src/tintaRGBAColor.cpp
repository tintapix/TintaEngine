#include "tintaGraphicaPrecompile.h"
#include "tintaRGBAColor.h"


namespace Graphica {

	const tintaRGBAColor tintaRGBAColor::BLACK(0.0f,0.0f,0.0f,1.0f);
	const tintaRGBAColor tintaRGBAColor::WHITE(1.0f,1.0f,1.0f,1.0f);
	const tintaRGBAColor tintaRGBAColor::INVALID(-1.0f,-1.0f,-1.0f,-1.0f);
	
	tintaRGBAColor::tintaRGBAColor ()	{
		m_color[0] = 0.0f;
		m_color[1] = 0.0f;
		m_color[2] = 0.0f;
		m_color[3] = 0.0f;
	}
	
	tintaRGBAColor::tintaRGBAColor (float fR, float fG, float fB, float fA){
		m_color[0] = fR;
		m_color[1] = fG;
		m_color[2] = fB;
		m_color[3] = fA;
	}
	
	tintaRGBAColor::tintaRGBAColor (const float* chann_arr){
		m_color[0] = chann_arr[0];
		m_color[1] = chann_arr[1];
		m_color[2] = chann_arr[2];
		m_color[3] = chann_arr[3];
	}
	
	tintaRGBAColor::tintaRGBAColor (const tintaRGBAColor& rval){
		m_color[0] = rval.m_color[0];
		m_color[1] = rval.m_color[1];
		m_color[2] = rval.m_color[2];
		m_color[3] = rval.m_color[3];
	}

	tintaRGBAColor::tintaRGBAColor (const tintaVector4f& val){
		m_color[0] = val.mx;
		m_color[1] = val.my;
		m_color[2] = val.mz;
		m_color[3] = val.mw;

	}
	
	tintaRGBAColor& tintaRGBAColor::operator= (const tintaRGBAColor& rval)	{
		m_color[0] = rval.m_color[0];
		m_color[1] = rval.m_color[1];
		m_color[2] = rval.m_color[2];
		m_color[3] = rval.m_color[3];
		return *this;
	}
	tintaRGBAColor& tintaRGBAColor::operator= (const tintaVector4f& r_val){
		m_color[0] = r_val.mx;
		m_color[1] = r_val.my;
		m_color[2] = r_val.mz;
		m_color[3] = r_val.mw;
		return *this;

	}

	
	bool tintaRGBAColor::operator== (const tintaRGBAColor& rval) const {
		return
			m_color[0] == rval.m_color[0] &&
			m_color[1] == rval.m_color[1] &&
			m_color[2] == rval.m_color[2] &&
			m_color[3] == rval.m_color[3];
	}
	
	bool tintaRGBAColor::operator!= (const tintaRGBAColor& rval) const
	{
		return
			m_color[0] != rval.m_color[0] ||
			m_color[1] != rval.m_color[1] ||
			m_color[2] != rval.m_color[2] ||
			m_color[3] != rval.m_color[3];
	}
	
	int tintaRGBAColor::compare (const tintaRGBAColor& rval) const {
		return memcmp(m_color,rval.m_color,4*sizeof(float));
	}
	

	bool tintaRGBAColor::operator< (const tintaRGBAColor& rval) const	{
		return compare(rval) < 0;
	}
	
	bool tintaRGBAColor::operator<= (const tintaRGBAColor& rval) const {
		return compare(rval) <= 0;
	}
	
	bool tintaRGBAColor::operator> (const tintaRGBAColor& rval) const	{
		return compare(rval) > 0;
	}
	
	bool tintaRGBAColor::operator>= (const tintaRGBAColor& rval) const {
		return compare(rval) >= 0;
	}
	
	tintaRGBAColor tintaRGBAColor::operator+ (const tintaRGBAColor& rval) const {
		return tintaRGBAColor(
			m_color[0] + rval.m_color[0],
			m_color[1] + rval.m_color[1],
			m_color[2] + rval.m_color[2],
			m_color[3] + rval.m_color[3]);
	}
	
	tintaRGBAColor tintaRGBAColor::operator- (const tintaRGBAColor& rval) const {
		return tintaRGBAColor(
			m_color[0] - rval.m_color[0],
			m_color[1] - rval.m_color[1],
			m_color[2] - rval.m_color[2],
			m_color[3] - rval.m_color[3]);
	}
	
	tintaRGBAColor tintaRGBAColor::operator* (const tintaRGBAColor& rval) const {
		return tintaRGBAColor(
			m_color[0]*rval.m_color[0],
			m_color[1]*rval.m_color[1],
			m_color[2]*rval.m_color[2],
			m_color[3]*rval.m_color[3]);
	}
	
	tintaRGBAColor tintaRGBAColor::operator* (float fScalar) const {
		return tintaRGBAColor(
			fScalar*m_color[0],
			fScalar*m_color[1],
			fScalar*m_color[2],
			fScalar*m_color[3]);
	}
	
    tintaRGBAColor operator* (float fScalar, const tintaRGBAColor& rval)	{
		return tintaRGBAColor(
			fScalar*rval.m_color[0],
			fScalar*rval.m_color[1],
			fScalar*rval.m_color[2],
			fScalar*rval.m_color[3]);
	}
	
	tintaRGBAColor& tintaRGBAColor::operator+= (const tintaRGBAColor& rval) {
		m_color[0] += rval.m_color[0];
		m_color[1] += rval.m_color[1];
		m_color[2] += rval.m_color[2];
		m_color[3] += rval.m_color[3];
		return *this;
	}
	
	tintaRGBAColor& tintaRGBAColor::operator-= (const tintaRGBAColor& rval) {
		m_color[0] -= rval.m_color[0];
		m_color[1] -= rval.m_color[1];
		m_color[2] -= rval.m_color[2];
		m_color[3] -= rval.m_color[3];
		return *this;
	}
	
	tintaRGBAColor& tintaRGBAColor::operator*= (const tintaRGBAColor& rval) {
		m_color[0] *= rval.m_color[0];
		m_color[1] *= rval.m_color[1];
		m_color[2] *= rval.m_color[2];
		m_color[3] *= rval.m_color[3];
		return *this;
	}
	
	tintaRGBAColor& tintaRGBAColor::operator*= (float fScalar) {
		m_color[0] *= fScalar;
		m_color[1] *= fScalar;
		m_color[2] *= fScalar;
		m_color[3] *= fScalar;
		return *this;
	}
	
	

}