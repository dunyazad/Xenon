#include <polyscope/polyscope.h>
#include <polyscope/point_cloud.h>
#include <vector>

int main() {
    // Polyscope 초기화
    polyscope::init();

    // 예시: 3D 점 데이터
    std::vector<glm::vec3> points;
    for (int i = 0; i < 1000; ++i) {
        float x = float(rand()) / RAND_MAX;
        float y = float(rand()) / RAND_MAX;
        float z = float(rand()) / RAND_MAX;
        points.emplace_back(x, y, z);
    }

    // Point cloud 등록
    polyscope::registerPointCloud("my point cloud", points);

    // Polyscope GUI 실행
    polyscope::show();

    return 0;
}
