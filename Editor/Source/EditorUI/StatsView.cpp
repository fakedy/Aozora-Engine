#include "StatsView.h"


StatsView::StatsView()
{

}

void StatsView::draw() {
    ImGuiIO& io = ImGui::GetIO();
	ImGui::Begin("Stats"); 
	ImGui::Text("Frame time: %f ms", io.DeltaTime);
    frameTimeGraph();
	ImGui::Text("Frame rate: %f fps", io.Framerate);
	ImGui::End();
}


void StatsView::frameTimeGraph()
{


    ImGuiIO& io = ImGui::GetIO();



    static float values[90] = { 0 };
    static int values_offset = 0;
    static double refresh_time = 0.0;
    if (refresh_time == 0.0)
        refresh_time = ImGui::GetTime();
    while (refresh_time < ImGui::GetTime())
    {
        //values[values_offset] = Time::deltaTime; // wack idk why its wrong :)
        values[values_offset] = io.DeltaTime;
        values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
        refresh_time += 1.0f / 60.0f;
    }
    ImGui::PlotLines("Frame time", values, IM_ARRAYSIZE(values), values_offset, NULL, 4.16f, 33.3f, ImVec2(180, 60));

    double avg = 0;
    for (int i = 0; i < IM_ARRAYSIZE(values); i++) {
        avg += values[i];
    }
    avg /= IM_ARRAYSIZE(values);
}
