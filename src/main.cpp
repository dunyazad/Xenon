#include <windows.h>

#include <iostream>
#include <vector>

#include <polyscope/polyscope.h>
#include <polyscope/point_cloud.h>
#include <polyscope/surface_mesh.h>

#include "Serialization.hpp"
#include "Monitor.h"

#include "CUDA/main.cuh"

int main() {
    //MaximizeConsoleWindowOnMonitor(1);

    polyscope::options::programName = "Xenon";
	//polyscope::options::guiTitle = "Xenon Point Cloud Viewer";
    polyscope::options::groundPlaneMode = polyscope::GroundPlaneMode::None;
	polyscope::view::bgColor = { 0.3f, 0.5f, 0.7f, 1.0f };
	polyscope::view::setNavigateStyle(polyscope::NavigateStyle::Free);

    polyscope::init();

	PLYFormat ply;
    ply.Deserialize("D:\\Resources\\3D\\PLY\\Compound_Full.ply");

    std::vector<glm::vec3> points;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> colors;

    for (size_t i = 0; i < ply.GetPoints().size() / 3; i++)
    {
        auto x = ply.GetPoints()[i * 3 + 0];
        auto y = ply.GetPoints()[i * 3 + 1];
        auto z = ply.GetPoints()[i * 3 + 2];
        points.emplace_back(x, y, z);

        auto nx = ply.GetNormals()[i * 3 + 0];
        auto ny = ply.GetNormals()[i * 3 + 1];
        auto nz = ply.GetNormals()[i * 3 + 2];
        normals.emplace_back(nx, ny, nz);

        if (ply.GetColors().size() == ply.GetPoints().size())
        {
            auto r = ply.GetColors()[i * 3 + 0];
            auto g = ply.GetColors()[i * 3 + 1];
            auto b = ply.GetColors()[i * 3 + 2];
            colors.emplace_back(r, g, b);
        }
        else if (ply.GetColors().size() / 4 == ply.GetPoints().size() / 3)
        {
            auto r = ply.GetColors()[i * 4 + 0];
            auto g = ply.GetColors()[i * 4 + 1];
            auto b = ply.GetColors()[i * 4 + 2];
            colors.emplace_back(r, g, b);
        }
        else
        {
            colors.emplace_back(1.0f, 1.0f, 1.0f);
		}
    }

    auto pcd = polyscope::registerPointCloud("point cloud", points);

    auto normalAttribute = pcd->addVectorQuantity("normals", normals, polyscope::VectorType::STANDARD);// ->setEnabled(true);
    normalAttribute->setVectorRadius(0.01, false);
    normalAttribute->setVectorLengthScale(0.1, false);
    
    pcd->addColorQuantity("color", colors);
    pcd->addColorQuantity("color", colors)->setEnabled(true);
    //pcd->setPointRenderMode(polyscope::PointRenderMode::Quad);
    pcd->setPointRadius(0.0005, true);


    vector<array<size_t, 3>> faces;
    for (size_t i = 0; i < ply.GetTriangleIndices().size() / 3; i++)
    {
        auto i0 = ply.GetTriangleIndices()[i * 3 + 0];
        auto i1 = ply.GetTriangleIndices()[i * 3 + 1];
        auto i2 = ply.GetTriangleIndices()[i * 3 + 2];

        faces.push_back({ i0, i1, i2 });
    }

	polyscope::SurfaceMesh* mesh = nullptr;

    if (!faces.empty()) {
        mesh = polyscope::registerSurfaceMesh("mesh", points, faces);
        mesh->addVertexColorQuantity("vertex color", colors)->setEnabled(true);
    }

    polyscope::state::userCallback = [&]() {
        if (ImGui::IsKeyPressed(ImGuiKey_Escape)) { // 'ESC'
            polyscope::shutdown(true);
        }
        if (ImGui::IsKeyPressed(ImGuiKey_Equal)) { // '+'
            double r = pcd->getPointRadius();
            pcd->setPointRadius(r * 1.1, false);
        }
        if (ImGui::IsKeyPressed(ImGuiKey_Minus)) { // '-'
            double r = pcd->getPointRadius();
            pcd->setPointRadius(r * 0.9, false);
        }
        if (ImGui::IsKeyPressed(ImGuiKey_GraveAccent)) { // '`'

            auto material = pcd->getMaterial();
            if ("clay" == material)
            {
                pcd->setMaterial("flat");
            }
            else if ("flat" == material)
            {
                pcd->setMaterial("clay");
            }

            printf("%s\n", pcd->getMaterial().c_str());
        }
        if (ImGui::IsKeyPressed(ImGuiKey_F1)) { // 'F1'
			pcd->setEnabled(!pcd->isEnabled());
        }
        if (ImGui::IsKeyPressed(ImGuiKey_F2)) { // 'F2'
            if (nullptr != mesh)
            {
                mesh->setEnabled(!mesh->isEnabled());
            }
        }
        if (ImGui::IsKeyPressed(ImGuiKey_Tab)) { // '\t'
            normalAttribute->setEnabled(!normalAttribute->isEnabled());
        }
    };

    CUDAMain();

    polyscope::show();

    return 0;
}
