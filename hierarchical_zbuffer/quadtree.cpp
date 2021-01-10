#include "quadtree.h"

QuadTree::QuadTree(int Width, int Height) {
	width = Width;
	height = Height;
	frameBuffer = new float[width * height];
	zBuffer = new float[width*height];
	memset(zBuffer, FLT_MIN, width*height * sizeof(float));
	root = new QuadTreeNode(1);

	buildQuadTree();
}

void QuadTree::buildQuadTree() {
	root->box = new QuadBoundingBox{ 0, width, 0, height, (width + 1) / 2, (height + 1) / 2 };
	root->z = FLT_MIN;
	splitNode(root);
}

void QuadTree::splitNode(QuadTreeNode* node) {
	QuadBoundingBox* box = node->box;
	if (box->xr - box->xl <= 1 && box->yr - box->yl <= 1)
		return;

	for (int i = 0; i < 4; ++i) {
		if ((box->xr <= box->centerX && i & 1) ||
			box->yr <= box->centerY && i & 2)
			continue;
		QuadTreeNode* nodeTemp = new QuadTreeNode((node->locCode << 2) | i);
		switch (i) {
		case 0:
			nodeTemp->box = new QuadBoundingBox{ box->xl, box->centerX, box->yl, box->centerY, (box->xl + box->centerX + 1) / 2, (box->yl + box->centerY + 1) / 2 };
			break;
		case 1:
			nodeTemp->box = new QuadBoundingBox{ box->centerX, box->xr, box->yl, box->centerY, (box->centerX + box->xr + 1) / 2, (box->yl + box->centerY + 1) / 2 };
			break;
		case 2:
			nodeTemp->box = new QuadBoundingBox{ box->xl, box->centerX, box->centerY, box->yr, (box->xl + box->centerX + 1) / 2, (box->centerY + box->yr + 1) / 2 };
			break;
		case 3:
			nodeTemp->box = new QuadBoundingBox{ box->centerX, box->xr, box->centerY, box->yr, (box->centerX + box->xr + 1) / 2, (box->centerY + box->yr + 1) / 2 };
			break;
		}
		nodeTemp->z = node->z;
		node->childExists |= (1 << i);
		nodes[nodeTemp->locCode] = *nodeTemp;
	}
	// 对子节点迭代
	for (int i = 0; i < 3; ++i) {
		if (node->childExists&(1 << i)) {
			const uint32_t locCodeChild = (node->locCode << 2) | i;
			// splitNode(&nodes[locCodeChild]);
			auto* child = lookupNode(locCodeChild);
			splitNode(child);
		}
	}
}

QuadTreeNode* QuadTree::getParentNode(QuadTreeNode* node) {
	const uint32_t locCodeParent = node->locCode >> 2;
	return lookupNode(locCodeParent);
}

QuadTreeNode* QuadTree::lookupNode(uint32_t locCode) {
	return &nodes[locCode];
}

size_t QuadTree::getNodeTreeDepth(const QuadTreeNode* node) {
	int depth = 0;
	for (uint32_t lc = node->locCode; lc != 1; lc >>= 2, ++depth);
	return depth;
}