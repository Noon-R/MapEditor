#pragma once
//Å•êF
typedef struct color_t{
	float red, green, blue, alpha;
	color_t() :red(1), green(1), blue(1), alpha(1) {}
	color_t(float red_, float green_, float blue_) :red(min(1, max(0, red_))), green(min(1, max(0, green_))), blue(min(1, max(0, blue_))), alpha(1) {}
	color_t(float red_, float green_, float blue_, float alpha_) :red(min(1,max(0,red_))), green(min(1,max(0,green_))), blue(min(1,max(0,blue_))), alpha(min(1,max(0,alpha_))) {}
} color_t;