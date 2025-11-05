//
// C++ Interface: imageview
//
// Description: 
//
//
// Author: Os3 s.r.l. <xgui@os3.it>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
namespace xgui
{
	class Image;

	struct ImageViewClass : public xgui::WidgetClass
	{
		ImageViewClass();
		std::string const &className();
		bool isInstanceable();
		xgui::Object * create(xgui::Object * parent, xgui::StringMap &properties);
	};

	class ImageView : public xgui::Widget
	{
		protected:
			ImageView ( xgui::Container * parent, xgui::Image * img );
			virtual ~ImageView();

		public:
			xgui::Image * getImage();
			void setImage ( xgui::Image * img );
			std::pair<int, int> getViewSize();
			bool isScrolled();
	};
}

%extend xgui::ImageView {
	%pythoncode {
		def __del__(self):
			if self.valid: self.delPyOwner(self)
	}
}		

