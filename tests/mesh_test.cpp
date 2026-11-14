// The library through the module alone: a grid of quads indexed, reordered,
// simplified, cut into meshlets, and through both codecs.
#include <gtest/gtest.h>

import std;
import mcpplibs.mwrap.meshoptimizer;

namespace {

struct Vertex {
    float x, y, z;
};

// side × side quads on the plane z = 0, each two triangles, every corner its own vertex.
std::vector<Vertex> soup(unsigned side) {
    std::vector<Vertex> out;
    for (unsigned row = 0; row < side; ++row)
        for (unsigned column = 0; column < side; ++column) {
            const float x = static_cast<float>(column), y = static_cast<float>(row);
            for (const auto& corner : {std::pair{0.f, 0.f}, {1.f, 0.f}, {1.f, 1.f}, {0.f, 0.f}, {1.f, 1.f}, {0.f, 1.f}}) out.push_back({x + corner.first, y + corner.second, 0});
        }
    return out;
}

struct Indexed {
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
};

Indexed indexed(unsigned side) {
    const std::vector<Vertex> corners = soup(side);
    std::vector<unsigned int> remap(corners.size());
    const std::size_t unique = meshopt_generateVertexRemap(remap.data(), nullptr, corners.size(), corners.data(), corners.size(), sizeof(Vertex));
    Indexed out{std::vector<Vertex>(unique), std::vector<unsigned int>(corners.size())};
    meshopt_remapIndexBuffer(out.indices.data(), nullptr, corners.size(), remap.data());
    meshopt_remapVertexBuffer(out.vertices.data(), corners.data(), corners.size(), sizeof(Vertex), remap.data());
    return out;
}

}  // namespace

TEST(Mesh, IndexingFindsSharedCorners) {
    const Indexed mesh = indexed(8);
    EXPECT_EQ(mesh.vertices.size(), 81u);
    EXPECT_EQ(mesh.indices.size(), 8u * 8u * 6u);
    for (const unsigned int index : mesh.indices) EXPECT_LT(index, mesh.vertices.size());
}

TEST(Mesh, VertexCacheOrderIsBetterAndTheSameTriangles) {
    Indexed mesh = indexed(16);
    std::mt19937 shuffle(7);
    std::vector<std::array<unsigned int, 3>> triangles(mesh.indices.size() / 3);
    std::memcpy(triangles.data(), mesh.indices.data(), mesh.indices.size() * sizeof(unsigned int));
    std::ranges::shuffle(triangles, shuffle);
    std::memcpy(mesh.indices.data(), triangles.data(), mesh.indices.size() * sizeof(unsigned int));
    const meshopt_VertexCacheStatistics before = meshopt_analyzeVertexCache(mesh.indices.data(), mesh.indices.size(), mesh.vertices.size(), 16, 0, 0);
    std::vector<unsigned int> ordered(mesh.indices.size());
    meshopt_optimizeVertexCache(ordered.data(), mesh.indices.data(), mesh.indices.size(), mesh.vertices.size());
    const meshopt_VertexCacheStatistics after = meshopt_analyzeVertexCache(ordered.data(), ordered.size(), mesh.vertices.size(), 16, 0, 0);
    EXPECT_LT(after.acmr, before.acmr);
    EXPECT_LT(after.acmr, 1.0f);
    std::vector<unsigned int> one = mesh.indices, other = ordered;
    std::ranges::sort(one), std::ranges::sort(other);
    EXPECT_EQ(one, other);
}

TEST(Mesh, SimplifyingAFlatGridLeavesFewTriangles) {
    const Indexed mesh = indexed(16);
    std::vector<unsigned int> fewer(mesh.indices.size());
    float error = -1;
    const std::size_t count = meshopt_simplify(fewer.data(), mesh.indices.data(), mesh.indices.size(), &mesh.vertices[0].x, mesh.vertices.size(), sizeof(Vertex), 6, 0.01f, 0, &error);
    EXPECT_EQ(count % 3, 0u);
    EXPECT_LE(count, 6u * 4u);
    EXPECT_GE(count, 6u);
    EXPECT_GE(error, 0.0f);
    EXPECT_LT(error, 0.01f);
}

TEST(Mesh, TheHeadersOverloadsTakeShortIndices) {
    const Indexed mesh = indexed(8);
    std::vector<unsigned short> narrow(mesh.indices.begin(), mesh.indices.end()), ordered(narrow.size());
    meshopt_optimizeVertexCache(ordered.data(), narrow.data(), narrow.size(), mesh.vertices.size());
    std::vector<unsigned int> wide(mesh.indices.size());
    meshopt_optimizeVertexCache(wide.data(), mesh.indices.data(), mesh.indices.size(), mesh.vertices.size());
    EXPECT_TRUE(std::ranges::equal(ordered, wide));
}

TEST(Mesh, MeshletsHoldEveryTriangleWithinTheirLimits) {
    const Indexed mesh = indexed(16);
    constexpr std::size_t most_vertices = 64, most_triangles = 96;
    std::vector<meshopt_Meshlet> meshlets(meshopt_buildMeshletsBound(mesh.indices.size(), most_vertices, most_triangles));
    std::vector<unsigned int>    meshlet_vertices(mesh.indices.size());
    std::vector<unsigned char>   meshlet_triangles(mesh.indices.size());
    meshlets.resize(meshopt_buildMeshlets(meshlets.data(), meshlet_vertices.data(), meshlet_triangles.data(), mesh.indices.data(), mesh.indices.size(), &mesh.vertices[0].x,
                                          mesh.vertices.size(), sizeof(Vertex), most_vertices, most_triangles, 0.0f));
    std::size_t triangles = 0;
    for (const meshopt_Meshlet& meshlet : meshlets) {
        EXPECT_LE(meshlet.vertex_count, most_vertices);
        EXPECT_LE(meshlet.triangle_count, most_triangles);
        triangles += meshlet.triangle_count;
        const meshopt_Bounds bounds = meshopt_computeMeshletBounds(&meshlet_vertices[meshlet.vertex_offset], &meshlet_triangles[meshlet.triangle_offset], meshlet.triangle_count,
                                                                   &mesh.vertices[0].x, mesh.vertices.size(), sizeof(Vertex));
        EXPECT_GT(bounds.radius, 0.0f);
        EXPECT_FLOAT_EQ(std::abs(bounds.cone_axis[2]), 1.0f);  // a flat grid: every normal along z
    }
    EXPECT_EQ(triangles, mesh.indices.size() / 3);
    EXPECT_GT(meshlets.size(), 1u);
}

TEST(Mesh, BothCodecsGiveBackTheBytes) {
    const Indexed mesh = indexed(12);
    std::vector<unsigned char> packed(meshopt_encodeVertexBufferBound(mesh.vertices.size(), sizeof(Vertex)));
    packed.resize(meshopt_encodeVertexBuffer(packed.data(), packed.size(), mesh.vertices.data(), mesh.vertices.size(), sizeof(Vertex)));
    EXPECT_LT(packed.size(), mesh.vertices.size() * sizeof(Vertex));
    std::vector<Vertex> vertices(mesh.vertices.size());
    ASSERT_EQ(meshopt_decodeVertexBuffer(vertices.data(), vertices.size(), sizeof(Vertex), packed.data(), packed.size()), 0);
    EXPECT_EQ(std::memcmp(vertices.data(), mesh.vertices.data(), vertices.size() * sizeof(Vertex)), 0);

    std::vector<unsigned char> packed_indices(meshopt_encodeIndexBufferBound(mesh.indices.size(), mesh.vertices.size()));
    packed_indices.resize(meshopt_encodeIndexBuffer(packed_indices.data(), packed_indices.size(), mesh.indices.data(), mesh.indices.size()));
    std::vector<unsigned int> indices(mesh.indices.size());
    ASSERT_EQ(meshopt_decodeIndexBuffer(indices.data(), indices.size(), packed_indices.data(), packed_indices.size()), 0);
    // The index codec may turn a triangle about (the same triangle, begun at another corner).
    for (std::size_t triangle = 0; triangle < indices.size(); triangle += 3) {
        std::array<unsigned int, 3> one{indices[triangle], indices[triangle + 1], indices[triangle + 2]}, other{mesh.indices[triangle], mesh.indices[triangle + 1], mesh.indices[triangle + 2]};
        std::ranges::sort(one), std::ranges::sort(other);
        EXPECT_EQ(one, other);
    }
}

TEST(Mesh, QuantizingAndTheVersion) {
    EXPECT_EQ(meshopt_quantizeUnorm(1.0f, 8), 255);
    EXPECT_EQ(meshopt_quantizeUnorm(0.5f, 8), 128);
    EXPECT_EQ(meshopt_quantizeSnorm(-1.0f, 8), -127);
    EXPECT_EQ(meshopt_quantizeHalf(1.0f), 0x3c00);
    EXPECT_EQ(mcpplibs::mwrap::meshoptimizer::macros::MESHOPTIMIZER_VERSION, 1030);
    EXPECT_EQ(meshopt_SimplifyLockBorder, 1);
    EXPECT_EQ(static_cast<int>(meshopt_EncodeExpSeparate), 0);
}
