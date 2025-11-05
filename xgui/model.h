//
// C++ Interface: model
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef _XGUI_MODEL_H_
#define _XGUI_MODEL_H_

#include "object.h"
#include <vector>
#include <string>
#include <set>

namespace xgui
{
	class View;
	class Master;

	struct ModelClass : public xgui::ObjectClass
	{
		ModelClass();
		std::string const &className();
		bool isInstanceable();
		bool isModel();
		bool mustFinalize();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
		void finalize(xgui::Object * o);

		virtual bool canContain(xgui::ClassInfo * c);
		virtual bool canBeContainedIn(xgui::ClassInfo * c);
	};

	class TreeNode : public xgui::Object
	{
		public:
			typedef std::vector<TreeNode *>::iterator iterator;
	
			TreeNode ();
			virtual ~TreeNode ();
	
			virtual int appendChild ( TreeNode * child );
			virtual void insertChild ( TreeNode * child, int pos );
			virtual void removeChild ( int pos );
			virtual void removeAllChildren();

			unsigned int size();
			std::string path();
	
			TreeNode * getChild ( int pos );
			virtual void setChild ( TreeNode * new_child, int pos );
			xgui::TreeNode * findByPath(std::string const &path);
			TreeNode * getParent();
	
			iterator begin ();
			iterator end();
	
			void ref();
			void unref();
	
		protected:
			long int                        _owncount;
			std::vector < TreeNode * >      _children;
			TreeNode *                      _parent;
	};

	class Model : public TreeNode
	{
		protected:
			std::set < View * >             view_list;
			std::vector < std::string >     string_list;
			unsigned long                   flags;
			std::string                     style_name;

			void linkView(View *view);
			void unlinkView(View *view);
			
			void insertInViews(Model * element_parent, int element_pos, Model * element);
			void removeInViews(Model * element_parent, int element_pos, Model * element);

			Model();
			Model ( const std::vector<std::string> & string_list );
			virtual ~Model();

			void destroy();

		public:
			void appendChild ( std::vector<std::string> const & string_list, std::string const &style_name = "default" );
			void appendChild ( std::string const & val, std::string const &style_name = "default" );
			int appendChild ( TreeNode * child );

			void insertChild ( TreeNode * child, int pos );
			void removeChild ( int pos );
			void removeAllChildren ();
			void setChild ( TreeNode * new_child, int pos );
			xgui::Model * getChild( int pos );
			xgui::Model * findByPath( std::string const &path );
			int findChildPos(Model * child);

			void setFlag(unsigned long flag);
			void unsetFlag(unsigned long flag);
			bool getFlagStatus(unsigned long flag);

			void setStyle ( std::string const & style_name );
			std::string const & getStyle();

			int addString(std::string const &str);
			void delString(int pos);
			int setString(std::string const &str, int pos);
			const std::string & getString ( int pos );
			unsigned int numberOfStrings();
			void clearStrings();

			void updateViews();

			std::string dump();

			friend class xgui::View;
			friend class Master;
	};
}

#endif

