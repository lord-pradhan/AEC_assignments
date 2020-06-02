#include "astar.h"


std::vector <YsShellExt::VertexHandle> A_Star(const YsShellExt &shl,YsShellExt::VertexHandle startVtHd,
	YsShellExt::VertexHandle goalVtHd)
{
	// std::unordered_map<int, double> lookUpG;
	std::unordered_set< int > closed_list;

	// initialize root of tree
	TreeNode* root = new TreeNode;
	root->setParent(nullptr);
    root->setG(0.0);
    root->setHandle(startVtHd);
    root->calcH(goalVtHd, shl);

	std::priority_queue <TreeNode*, std::vector<TreeNode*>, CompareF> open_set;
	open_set.push(root);

	while( !open_set.empty() && !(open_set.top()->getHandle() == goalVtHd) ){

		TreeNode* tempPtr = open_set.top();
        open_set.pop();
        closed_list.insert( shl.GetSearchKey( tempPtr->getHandle() ) );

        auto vertArray = shl.GetConnectedVertex(tempPtr->getHandle());

        for(auto i : vertArray){

        	if(closed_list.find(shl.GetSearchKey(i)) == closed_list.end()){

        		TreeNode* succNode = new TreeNode;
        		succNode->setParent(tempPtr);
        		succNode->setHandle(i);

        		YsVec3 currPos, prevPos;
        		shl.GetVertexPosition(currPos, i);
        		shl.GetVertexPosition(prevPos, tempPtr->getHandle());
        		succNode->setG( tempPtr->getG() + distanceFn( currPos, prevPos ) );
        		succNode->calcH(goalVtHd, shl);

        		tempPtr->addSuccesor(succNode);        		

        		open_set.push(succNode);
        	}
        }
	}

	if(open_set.empty()){

		std::cout<< "Open set is empty, path cannot be found"<<std::endl;
		return std::vector <YsShellExt::VertexHandle>(); // Path not found.

	} 
	else{

		std::cout<< "Goal expanded, start reconstruction"<<std::endl;
		std::vector<YsShellExt::VertexHandle> optPath;
		
		TreeNode* traverse = open_set.top();

		while(traverse->getParent()!=nullptr){

			optPath.push_back(traverse->getHandle());
			traverse = traverse->getParent();
        }

        return optPath;
	}
}

std::vector <YsShellExt::VertexHandle> A_Star_ReconstructPath(
    const YsShellExt &shl,const std::unordered_map <YSHASHKEY,YsShellExt::VertexHandle> &cameFrom,
    YsShellExt::VertexHandle current)
{	
	std::vector <YsShellExt::VertexHandle> path;
	return path;
}


double distanceFn(YsVec3 currPos, YsVec3 goalPos){

	return sqrt( (currPos.x() - goalPos.x())*(currPos.x() - goalPos.x()) + 
			(currPos.y() - goalPos.y())*(currPos.y() - goalPos.y()) + 
			(currPos.z() - goalPos.z())*(currPos.z() - goalPos.z()) );
}