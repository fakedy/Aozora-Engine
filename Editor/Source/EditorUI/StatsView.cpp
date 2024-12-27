#include "StatsView.h"


StatsView::StatsView()
{

}

void StatsView::draw() {

	ImGui::Begin("Stats"); // will display components
	ImGui::Text("Frame time: %f ms", Time::deltaTime);
    frameTimeGraph();
	ImGui::Text("Frame rate: %f fps", 1000 / Time::deltaTime);
	ImGui::End();
}


void StatsView::frameTimeGraph()
{
    static float values[90] = { 0 };
    static int values_offset = 0;
    static double refresh_time = 0.0;
    if (refresh_time == 0.0)
        refresh_time = ImGui::GetTime();
    while (refresh_time < ImGui::GetTime())
    {
        values[values_offset] = Time::deltaTime;
        values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
        refresh_time += 1.0f / 60.0f;
    }
    ImGui::PlotLines("Frame time", values, IM_ARRAYSIZE(values), values_offset, NULL, 0.0f, 33.0f, ImVec2(180, 60));

    double avg = 0;
    for (int i = 0; i < IM_ARRAYSIZE(values); i++) {
        avg += values[i];
    }
    avg /= IM_ARRAYSIZE(values);
    ImGui::Text("Average frametime: ");
    ImGui::SameLine();
    ImGui::Text(std::to_string(avg).c_str());
}
