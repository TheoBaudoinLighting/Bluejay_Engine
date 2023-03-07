// Color class 
#pragma once

#include <iostream>

#include "bje_vec3.h"


/*Color conversions are useful for accurately representing colors in a graphic rendering.
 *By using different color spaces and appropriate conversion formulas, it is possible to
 *represent colors in formats that take into account the perceptual properties of color,
 *such as brightness, saturation and hue.
 * In a graphic rendering, it is important to take into
 * account the properties of reflection and transmission of materials,
 * as well as the perceptual properties of the color.Color conversions
 * allow to take into account these properties in a more precise way, which can improve
 * the quality and the precision of the rendering.

 *For example, in spectral rendering, color conversions can be used to account for the sensitivity
 *of the human eye to different colors, which can improve the accuracy and
 *consistency of rendered colors.*/


// RGBA Struct
struct RGBA
{
	// Constructor
	unsigned char r, g, b, a;
	RGBA() = default;

	constexpr RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {}
};

class Color
{
	// http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
	// http://www.brucelindbloom.com/index.html?Eqn_XYZ_to_Lab.html
	// http://www.brucelindbloom.com/index.html?Eqn_Lab_to_XYZ.html
	// http://www.brucelindbloom.com/index.html?Eqn_XYZ_to_RGB.html
	// http://www.brucelindbloom.com/index.html?Eqn_RGB_to_XYZ.html
	// http://www.brucelindbloom.com/index.html?Eqn_RGB_to_Lab.html
	// http://www.brucelindbloom.com/index.html?Eqn_Lab_to_RGB.html
	// http://www.brucelindbloom.com/index.html?Eqn_RGB_to_HSV.html
	// http://www.brucelindbloom.com/index.html?Eqn_HSV_to_RGB.html
	// http://www.brucelindbloom.com/index.html?Eqn_RGB_to_HSL.html
	// http://www.brucelindbloom.com/index.html?Eqn_HSL_to_RGB.html
	// http://www.brucelindbloom.com/index.html?Eqn_RGB_to_Yxy.html
	// http://www.brucelindbloom.com/index.html?Eqn_Yxy_to_RGB.html
	// http://www.brucelindbloom.com/index.html?Eqn_RGB_to_YCbCr.html
	// http://www.brucelindbloom.com/index.html?Eqn_YCbCr_to_RGB.html
	// http://www.brucelindbloom.com/index.html?Eqn_RGB_to_YDbDr.html
	// http://www.brucelindbloom.com/index.html?Eqn_YDbDr_to_RGB.html
	// http://www.brucelindbloom.com/index.html?Eqn_RGB_to_CMY.html
	// http://www.brucelindbloom.com/index.html?Eqn_CMY_to_RGB.html

	// http://www.brucelindbloom.com/index.html?WorkingSpaceInfo.html
	// http://www.brucelindbloom.com/index.html?ColorChecker.html
	// http://www.brucelindbloom.com/index.html?ColorCalculator.html
	// http://www.brucelindbloom.com/index.html?ColorDifference.html
	// http://www.brucelindbloom.com/index.html?ColorDifferenceChart.html

	// One Week RayTracing
	// https://raytracing.github.io/books/RayTracingInOneWeekend.html // For the color class base code



	// XYZ to RGB RGB = R + G + B
	color xyz_to_srgb(const color& xyz)
	{
		return color( 3.2404542 * xyz.x() - 1.5371385 * xyz.y() - 0.4985314 * xyz.z(),
		             -0.9692660 * xyz.x() + 1.8760108 * xyz.y() + 0.0415560 * xyz.z(),
			          0.0556434 * xyz.x() - 0.2040259 * xyz.y() + 1.0572252 * xyz.z() );
	}

	// RGB to XYZ XYZ = X + Y + Z
	color srgb_to_xyz(const color& srgb)
	{
		return color(
			0.4124564 * srgb.x() + 0.3575761 * srgb.y() + 0.1804375 * srgb.z(),
			0.2126729 * srgb.x() + 0.7151522 * srgb.y() + 0.0721750 * srgb.z(),
			0.0193339 * srgb.x() + 0.1191920 * srgb.y() + 0.9503041 * srgb.z()
		);
	}

	// LAB to RGB
	color xyz_to_lab(const color& xyz)
	{
		color lab;
		lab[0] = 116 * f(xyz[1] / 100) - 16;
		lab[1] = 500 * (f(xyz[0] / 95.047) - f(xyz[1] / 100));
		lab[2] = 200 * (f(xyz[1] / 100) - f(xyz[2] / 108.883));
		return lab;
	}

	// RGB to LAB 
	color lab_to_xyz(const color& lab)
	{
		color xyz;
		xyz[1] = (lab[0] + 16) / 116;
		xyz[0] = lab[1] / 500 + xyz[1];
		xyz[2] = xyz[1] - lab[2] / 200;
		xyz[0] = 95.047 * f(xyz[0]);
		xyz[1] = 100 * f(xyz[1]);
		xyz[2] = 108.883 * f(xyz[2]);
		return xyz;
	}

	// LAB to RGB LAB = Luminosity + Achromatic + Bchromatic
	color lab_to_srgb(const color& lab)
	{
		return xyz_to_srgb(lab_to_xyz(lab));
	}

	// RGB to LAB
	color srgb_to_lab(const color& srgb)
	{
		return xyz_to_lab(srgb_to_xyz(srgb));
	}

	// LCH to RGB  LCH = Luminosity + Chroma + Hue
	color srgb_to_lch(const color& srgb)
	{
		color lab = srgb_to_lab(srgb);
		color lch;
		lch[0] = lab[0];
		lch[1] = sqrt(lab[1] * lab[1] + lab[2] * lab[2]);
		lch[2] = atan2(lab[2], lab[1]) * 180 / M_PI;
		return lch;
	}

	// RGB to LCH
	color lch_to_srgb(const color& lch)
	{
		color lab;
		lab[0] = lch[0];
		lab[1] = lch[1] * cos(lch[2] * M_PI / 180);
		lab[2] = lch[1] * sin(lch[2] * M_PI / 180);
		return lab_to_srgb(lab);
	}

	// HSV to RGB HSV = Hue + Saturation + Value
	color srgb_to_hsv(const color& srgb)
	{
		color hsv;
		float min = std::min(srgb[0], std::min(srgb[1], srgb[2]));
		float max = std::max(srgb[0], std::max(srgb[1], srgb[2]));
		hsv[2] = max;
		float delta = max - min;
		if (max != 0)
			hsv[1] = delta / max;
		else
		{
			hsv[1] = 0;
			hsv[0] = -1;
			return hsv;
		}
		if (srgb[0] == max)
			hsv[0] = (srgb[1] - srgb[2]) / delta;
		else if (srgb[1] == max)
			hsv[0] = 2 + (srgb[2] - srgb[0]) / delta;
		else
			hsv[0] = 4 + (srgb[0] - srgb[1]) / delta;
		hsv[0] *= 60;
		if (hsv[0] < 0)
			hsv[0] += 360;
		return hsv;
	}

	color hsv_to_srgb(const color& hsv)
	{
		color srgb;
		int i;
		float f, p, q, t;
		if (hsv[1] == 0)
		{
			srgb[0] = hsv[2];
			srgb[1] = hsv[2];
			srgb[2] = hsv[2];
			return srgb;
		}
		float h = hsv[0] / 60;
		i = floor(h);
		f = h - i;
		p = hsv[2] * (1 - hsv[1]);
		q = hsv[2] * (1 - hsv[1] * f);
		t = hsv[2] * (1 - hsv[1] * (1 - f));
		switch (i)
		{
		case 0:
			srgb[0] = hsv[2];
			srgb[1] = t;
			srgb[2] = p;
			break;
		case 1:
			srgb[0] = q;
			srgb[1] = hsv[2];
			srgb[2] = p;
			break;
		case 2:
			srgb[0] = p;
			srgb[1] = hsv[2];
			srgb[2] = t;
			break;
		case 3:
			srgb[0] = p;
			srgb[1] = q;
			srgb[2] = hsv[2];
			break;
		case 4:
			srgb[0] = t;
			srgb[1] = p;
			srgb[2] = hsv[2];
			break;
		default:
			srgb[0] = hsv[2];
			srgb[1] = p;
			srgb[2] = q;
			break;
		}
		return srgb;
	}

	// HSL to RGB HSL = Hue + Saturation + Luminosity
	color srgb_to_hsl(const color& srgb)
	{
		color hsl;
		float min = std::min(srgb[0], std::min(srgb[1], srgb[2]));
		float max = std::max(srgb[0], std::max(srgb[1], srgb[2]));
		hsl[2] = (max + min) / 2;
		float delta = max - min;
		if (max == min)
		{
			hsl[1] = 0;
			hsl[0] = -1;
			return hsl;
		}
		if (hsl[2] < 0.5)
			hsl[1] = delta / (max + min);
		else
			hsl[1] = delta / (2 - max - min);
		if (srgb[0] == max)
			hsl[0] = (srgb[1] - srgb[2]) / delta;
		else if (srgb[1] == max)
			hsl[0] = 2 + (srgb[2] - srgb[0]) / delta;
		else
			hsl[0] = 4 + (srgb[0] - srgb[1]) / delta;
		hsl[0] *= 60;
		if (hsl[0] < 0)
			hsl[0] += 360;
		return hsl;
	}


	color hsl_to_srgb(const color& hsl)
	{
		color srgb;
		if (hsl[1] == 0)
		{
			srgb[0] = hsl[2];
			srgb[1] = hsl[2];
			srgb[2] = hsl[2];
			return srgb;
		}
		float q;
		if (hsl[2] < 0.5)
			q = hsl[2] * (1 + hsl[1]);
		else
			q = hsl[2] + hsl[1] - hsl[2] * hsl[1];
		float p = 2 * hsl[2] - q;
		float hk = hsl[0] / 360;
		float t[3];
		t[0] = hk + 1.0 / 3;
		t[1] = hk;
		t[2] = hk - 1.0 / 3;
		for (int i = 0; i < 3; i++)
		{
			if (t[i] < 0)
				t[i] += 1;
			if (t[i] > 1)
				t[i] -= 1;
			if (t[i] < 1.0 / 6)
				srgb[i] = p + (q - p) * 6 * t[i];
			else if (t[i] < 0.5)
				srgb[i] = q;
			else if (t[i] < 2.0 / 3)
				srgb[i] = p + (q - p) * (2.0 / 3 - t[i]) * 6;
			else
				srgb[i] = p;
		}
		return srgb;
	}

	// RGB to HSI HSI = Hue + Saturation + Intensity
	color srgb_to_hsi(const color& srgb)
	{
		color hsi;
		float min = std::min(srgb[0], std::min(srgb[1], srgb[2]));
		float max = std::max(srgb[0], std::max(srgb[1], srgb[2]));
		float delta = max - min;
		hsi[2] = (max + min) / 2;
		if (max == min)
		{
			hsi[1] = 0;
			hsi[0] = -1;
			return hsi;
		}
		if (hsi[2] < 0.5)
			hsi[1] = delta / (max + min);
		else
			hsi[1] = delta / (2 - max - min);
		if (srgb[0] == max)
			hsi[0] = (srgb[1] - srgb[2]) / delta;
		else if (srgb[1] == max)
			hsi[0] = 2 + (srgb[2] - srgb[0]) / delta;
		else
			hsi[0] = 4 + (srgb[0] - srgb[1]) / delta;
		hsi[0] *= 60;
		if (hsi[0] < 0)
			hsi[0] += 360;
		return hsi;
	}

	color hsi_to_srgb(const color& hsi)
	{
		color srgb;
		float h = hsi[0] / 360;
		float s = hsi[1];
		float i = hsi[2];
		float r, g, b;
		if (h < 1.0 / 3)
		{
			b = (1 - s) / 3;
			r = (1 + s * cos(PI * h) / cos(PI / 3 - PI * h)) / 3;
			g = 1 - (b + r);
		}
		else if (h < 2.0 / 3)
		{
			h -= 1.0 / 3;
			r = (1 - s) / 3;
			g = (1 + s * cos(PI * h) / cos(PI / 3 - PI * h)) / 3;
			b = 1 - (r + g);
		}
		else
		{
			h -= 2.0 / 3;
			g = (1 - s) / 3;
			b = (1 + s * cos(PI * h) / cos(PI / 3 - PI * h)) / 3;
			r = 1 - (g + b);
		}
		srgb[0] = r + i * (1 + s * cos(PI * hsi[0]) / cos(PI / 3 - PI * hsi[0]));
		srgb[1] = g + i * (1 + s * cos(PI * (hsi[0] - 120)) / cos(PI / 3 - PI * (hsi[0] - 120)));
		srgb[2] = b + i * (1 + s * cos(PI * (hsi[0] - 240)) / cos(PI / 3 - PI * (hsi[0] - 240)));
		return srgb;
	}

	// RGB to HSL HSL = Hue + Saturation + Lightness
	color srgb_to_hsp(const color& srgb)
	{
		color hsp;
		float min = std::min(srgb[0], std::min(srgb[1], srgb[2]));
		float max = std::max(srgb[0], std::max(srgb[1], srgb[2]));
		float delta = max - min;
		hsp[2] = sqrt(0.299 * srgb[0] * srgb[0] + 0.587 * srgb[1] * srgb[1] + 0.114 * srgb[2] * srgb[2]);
		if (max == min)
		{
			hsp[1] = 0;
			hsp[0] = -1;
			return hsp;
		}
		if (hsp[2] < 0.5)
			hsp[1] = delta / (max + min);
		else
			hsp[1] = delta / (2 - max - min);
		if (srgb[0] == max)
			hsp[0] = (srgb[1] - srgb[2]) / delta;
		else if (srgb[1] == max)
			hsp[0] = 2 + (srgb[2] - srgb[0]) / delta;
		else
			hsp[0] = 4 + (srgb[0] - srgb[1]) / delta;
		hsp[0] *= 60;
		if (hsp[0] < 0)
			hsp[0] += 360;
		return hsp;
	}

	color hsp_to_srgb(const color& hsp)
	{
		color srgb;
		float h = hsp[0] / 360;
		float s = hsp[1];
		float p = hsp[2] * (1 - s);
		float q = hsp[2] * (1 - s * h);
		float t = hsp[2] * (1 - s * (1 - h));
		if (h < 1.0 / 6)
		{
			srgb[0] = hsp[2];
			srgb[1] = t;
			srgb[2] = p;
		}
		else if (h < 2.0 / 6)
		{
			srgb[0] = q;
			srgb[1] = hsp[2];
			srgb[2] = p;
		}
		else if (h < 3.0 / 6)
		{
			srgb[0] = p;
			srgb[1] = hsp[2];
			srgb[2] = t;
		}
		else if (h < 4.0 / 6)
		{
			srgb[0] = p;
			srgb[1] = q;
			srgb[2] = hsp[2];
		}
		else if (h < 5.0 / 6)
		{
			srgb[0] = t;
			srgb[1] = p;
			srgb[2] = hsp[2];
		}
		else
		{
			srgb[0] = hsp[2];
			srgb[1] = p;
			srgb[2] = q;
		}
		return srgb;
	}

	// RGB to CIE XYZ CIE XYZ = CIE X + CIE Y + CIE Z
	// RGB to CIE L*a*b* CIE L*a*b* = CIE L* + CIE a* + CIE b*
	color srgb_to_cie(const color& srgb)
	{
		color cie;
		cie[0] = 0.412453 * srgb[0] + 0.357580 * srgb[1] + 0.180423 * srgb[2];
		cie[1] = 0.212671 * srgb[0] + 0.715160 * srgb[1] + 0.072169 * srgb[2];
		cie[2] = 0.019334 * srgb[0] + 0.119193 * srgb[1] + 0.950227 * srgb[2];
		return cie;
	}

	color cie_to_srgb(const color& cie)
	{
		color srgb;
		srgb[0] = 3.240479 * cie[0] - 1.537150 * cie[1] - 0.498535 * cie[2];
		srgb[1] = -0.969256 * cie[0] + 1.875992 * cie[1] + 0.041556 * cie[2];
		srgb[2] = 0.055648 * cie[0] - 0.204043 * cie[1] + 1.057311 * cie[2];
		return srgb;
	}
	
	// Write the color to the image
	void write_color(struct RGBA& out, color pixel_color, int samples_per_pixel)
	{
		auto r = pixel_color[0];
		auto g = pixel_color[1];
		auto b = pixel_color[2];

		// Check for NaN values
		if (r != r) r = 0.0;
		if (g != g) g = 0.0;
		if (b != b) b = 0.0;

		// Calc the average color
		r = sqrt(r / samples_per_pixel);
		g = sqrt(g / samples_per_pixel);
		b = sqrt(b / samples_per_pixel);

		out.r = static_cast<unsigned char>(256 * clamp(r, 0.0, 0.999));
		out.g = static_cast<unsigned char>(256 * clamp(g, 0.0, 0.999));
		out.b = static_cast<unsigned char>(256 * clamp(b, 0.0, 0.999));
		out.a = 255;
	}
};
