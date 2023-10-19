#pragma once

#include "pch.h"

namespace Core
{
	namespace Math
	{
		template<typename TData>
		class Tree
		{
		public:
			void MapDFS();
			void MapBFS();

		private:
			struct InternalNode
			{
			public:
				InternalNode(InternalNode* inParent, const TData& inData)
					: parent{ inParent }
					, data{ inData }
				{

				}

				void AddChild(TNode node)
				{
					children.emplace(node);
				}

			public:
				InternalNode* parent{ nullptr };
				TDate data{ };
				std::unordered_set<InternalNode> children{ };
			};

		private:
		};
	}
}
