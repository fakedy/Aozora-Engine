#pragma once
#include "AozoraAPI/Aozora.h"
#include <imgui/imgui.h>
#include <Context.h>


class StatsView {
public:
	StatsView();
	void draw(const Aozora::Context& context);
	void frameTimeGraph();
private:

};