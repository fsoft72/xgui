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

%ignore xgui::TreeNode;

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
	
			virtual void appendChild ( TreeNode * child );
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
	};

	class Model : public TreeNode
	{
		protected:
			Model();
			virtual ~Model();

		public:
			void appendChild ( std::vector<std::string> const & string_list, std::string const &style_name = "default" );
			void appendChild ( std::string const & val, std::string const &style_name = "default" );
			void appendChild ( TreeNode * child );

			void insertChild ( TreeNode * child, int pos );
			void removeChild ( int pos );
			void removeAllChildren ();
			void setChild ( TreeNode * new_child, int pos );
			xgui::Model * getChild( int pos );
			xgui::Model * findByPath( std::string const &path );

			void setFlag(unsigned long flag);
			void unsetFlag(unsigned long flag);
			bool getFlagStatus(unsigned long flag);

			void setStyle ( std::string const & style_name );
			std::string const & getStyle();

			void addString(std::string const &str);
			void delString(int pos);
			void setString(std::string const &str, int pos);
			const std::string & getString ( int pos );
			void clearStrings();
			unsigned int numberOfStrings();

			void updateViews();

			unsigned int size();
	};
}
