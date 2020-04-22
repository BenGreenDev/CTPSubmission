#pragma once

#define GFX_THROW_FAILED(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )

// this.GetInfoManager() must exist
// this.GetInfoManager(Graphics& gfx) must exist
#ifdef NDEBUG
#define INFOMAN() HRESULT hr
#define INFOMAN(gfx) HRESULT hr
#else
#define INFOMAN() HRESULT hr;
#define INFOMAN(gfx) HRESULT hr;
#endif
