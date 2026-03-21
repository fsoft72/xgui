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
			typedef std::vector<TreeNode *>::const_iterator const_iterator;
	
			TreeNode ();
			virtual ~TreeNode ();
	
			virtual int appendChild ( TreeNode * child );
			virtual void insertChild ( TreeNode * child, int pos );
			virtual void removeChild ( int pos );
			virtual void removeAllChildren();

			unsigned int size() const;
			std::string path() const;
	
			TreeNode * getChild ( int pos ) const;
			virtual void setChild ( TreeNode * new_child, int pos );
			xgui::TreeNode * findByPath(std::string const &path) const;
			TreeNode * getParent() const;
	
			iterator begin ();
			const_iterator begin () const;
			iterator end();
			const_iterator end() const;
	
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
			xgui::Model * getChild( int pos ) const;
			xgui::Model * findByPath( std::string const &path ) const;
			int findChildPos(Model * child) const;

			void setFlag(unsigned long flag);
			void unsetFlag(unsigned long flag);
			bool getFlagStatus(unsigned long flag) const;

			void setStyle ( std::string const & style_name );
			std::string const & getStyle() const;

			int addString(std::string const &str);
			void delString(int pos);
			int setString(std::string const &str, int pos);
			const std::string & getString ( int pos ) const;
			unsigned int numberOfStrings() const;
			void clearStrings();

			void updateViews();

			std::string dump();

			friend class xgui::View;
			friend class Master;
	};
}

#endif

