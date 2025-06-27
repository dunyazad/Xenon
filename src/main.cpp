#include <polyscope/polyscope.h>
#include <polyscope/point_cloud.h>
#include <vector>

int main() {
    // Polyscope �ʱ�ȭ
    polyscope::init();

    // ����: 3D �� ������
    std::vector<glm::vec3> points;
    for (int i = 0; i < 1000; ++i) {
        float x = float(rand()) / RAND_MAX;
        float y = float(rand()) / RAND_MAX;
        float z = float(rand()) / RAND_MAX;
        points.emplace_back(x, y, z);
    }

    // Point cloud ���
    polyscope::registerPointCloud("my point cloud", points);

    // Polyscope GUI ����
    polyscope::show();

    return 0;
}
