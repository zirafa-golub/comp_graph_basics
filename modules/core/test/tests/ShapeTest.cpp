#include "core/Shape.h"

#include "gtest/gtest.h"

#include "core/TestUtils.h"

using namespace cg;

class TestShape : public cg::Shape {
public:
    const cg::MeshData& meshData() const override { return meshData_; }

private:
    cg::MeshData meshData_ = cg::MeshData({}, {}, {});
};

struct TestShaderGroup : public cg::ShaderGroup {};

TEST(ShapeTest, setShaderPackage_shouldSetShapeToPackage) {
    TestShape shape;
    std::unique_ptr<ShaderGroup> sg = std::make_unique<TestShaderGroup>();
    ShaderGroup* sgPtr = sg.get();
    shape.setShaderGroup(std::move(sg));

    EXPECT_EQ(&shape.shaderGroup(), sgPtr);
    EXPECT_EQ(&sgPtr->shape(), &shape);
}
