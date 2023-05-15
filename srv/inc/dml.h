//| ============================================================
//! @file
//!
//!	HTML output makers.
//!
//! @author Manfred Sorgo
//!         http://sorgo.de
//!
//| ============================================================

#ifndef DML_H
#define DML_H

#include "nob.h"
#include "ccs.h"
#include "sfunc.h"
#include "outstream.h"
#include "stypes.h"

//!	tag stream struct.
	struct ttag
	{
	//!	tag name.
		t_cc name;
	//!	store tag name for closing.
		bool st;
	};

//!	tag stream struct for tags that don't need a closing tag.
	struct stag { t_cc name; };

//!	attribute stream struct.
	template <class T>
	struct taa
	{
	//!	attribute name.
		t_cc name;
	//!	attribute value.
		const T& val;
	//!	forced quoting (e.g. "value" that can be user input)
		bool q;
	};

//!	attribute stream struct: numerical
	struct taan
	{
	//!	attribute name.
		t_cc name;
	//!	attribute value.
		const t_num val;
	//!	forced quoting (e.g. "value" that can be user input)
		bool q;
	};

//!	attribute stream struct: string.
	struct taac
	{
	//!	attribute name.
		t_cc name;
	//!	attribute value.
		t_cc val;
	//!	forced quoting (e.g. "value" that can be user input)
		bool q;
	};
//!	non value attribute (e.g. selected).
	struct saa { t_cc name; };

//!	a literal const attribute string.
//!	e.g. 'onfocus="blur();"'
	struct laa { t_cc str; };

//! functor object attribute.
	struct sfaa {
		t_cc name;
		sfunc* sf;
	};

//!	attribute stream opener.
	struct oaa { t_cc name; };

//!	attribute stream closer.
	struct eaa {};


	class dmlstream : public outstream
	{
	public:

	//!	open tag.
		dmlstream& operator <<(ttag t);
	//!	open single tag.
		dmlstream& operator <<(stag t);
	//!	tag end.
		dmlstream& operator <<(tt_end);
	//! tag attributes.
		template <class T>
		inline dmlstream& operator <<(taa<T> a)	{ if (open) attr(a); return *this; }
		inline dmlstream& operator <<(taan a)	{ if (open) attr(a); return *this; }
		inline dmlstream& operator <<(taac a)	{ if (open) attr(a); return *this; }
	//!	non value attribute.
		inline dmlstream& operator <<(saa a) 	{ if (open) attr(a); return *this; }
	//!	attributes literal.
		inline dmlstream& operator <<(laa a) 	{ if (open) attr(a); return *this; }
	//!	functor attribute object.
		inline dmlstream& operator <<(sfaa a)	{ if (open) attr(a); return *this; }
	//!	attribute stream opener.
		inline dmlstream& operator <<(oaa a) 	{ if (open) attr(a); return *this; }
	//!	attribute stream closer.
		inline dmlstream& operator <<(eaa a)	{ if (open) attr(a); return *this; }
	//!	tag null.
		inline dmlstream& operator <<(dmlstream&) { return *this; }
	//!	sfunc pointer
		inline dmlstream& operator <<(sfunc* psf) { check_open(); add(psf);  return *this; }

	//	standard types.
		template <class T> inline dmlstream& operator<<(const T& t) { check_open(); add(t);  return *this; }

	//	output and casts
	//!	reset output buffer.
		inline virtual void clear() { bfout.clear(); tbf.clear(); }
	//!	retieve output buffer data.
		inline t_cc str() 			{ return bfout; }
	//!	retieve output buffer data size.
		inline unsigned size() 		{ return bfout.size(); }

		inline t_cc last() 			{ return tbf.last(); }
		inline bool operator==(t_cc c) 	{ return tbf.last() && tbf.last() == c; }

		inline operator bool() 		{ return bfout; }
		inline operator t_cc()		{ return bfout; }

		static dmlstream& instance();
//		static dmlstream& tmp_instance();

	private:
		dmlstream(unsigned);
	//!	output buffer.
		cbuffer bfout;
	//!	tag stack buffer.
		sbuffer tbf;

		inline void add(t_cc c) 			{ bfout.add(c); }
		inline void add(char c)				{ bfout.add(c); }
		inline void add(t_num i)			{ bfout.add(i); }
		inline void add(const xbuffer& b) 	{ bfout.add(b.str()); }

		inline void add(const nob& n)   	{ n.put(bfout); }
		inline void add(const inob& n)  	{ n.put(bfout); }
		inline void add(const cnob& n)  	{ n.put(bfout); }
		inline void add(const cinob& n) 	{ n.put(bfout); }

		inline void	add(sfunc* psf)	{ (*psf)(*this); delete psf; }

		template <class T> inline void addq(taa<T> a);

		void attr(taa<t_cc>  a);
		void attr(taa<nob>   a);
		void attr(taa<t_num> a);
		void attr(taan       a);
		void attr(taac 		 a);
		void attr(saa		 a);
		void attr(laa		 a);
		void attr(oaa		 a);
		void attr(eaa		 a);
		void attr(sfaa		 a);

		bool open;
		bool aopen;
		bool sgl;
		void check_open();
	}; // dmlstream

	extern dmlstream& dml;
//	extern dmlstream& tmpdml;


//!	tag namespace
namespace tag
{
	inline ttag mtt(t_cc n, bool s = true) 	{ ttag t = { n, s }; return t; }
	inline stag mts(t_cc n) 				{ stag t = { n }; 	return t; }

	inline ttag div		() { return mtt(c_div); }
	inline ttag label	() { return mtt(c_label ); }
	inline ttag li		() { return mtt("li"); }
	inline ttag option	() { return mtt(c_option); }
	inline ttag p 		() { return mtt("p"); }
	inline ttag select	() { return mtt(c_select); }
	inline ttag span	() { return mtt(c_span); }
	inline ttag ul		() { return mtt("ul"); }

	inline stag br		() { return mts("br"); }
	inline stag img		() { return mts(c_img); }
	inline stag input	() { return mts(c_input); }

	inline ttag script	(bool s = true) { return mtt(c_script, s); }


} // namespace tag


//!	predefinition of attributes that have to be quoted
#define c_QUOTED_ATTS c_href, c_src, c_title, c_value

//!	tag attribute namespace
namespace ta
{
//!	indication whether the value of an attribute must be quoted.
//!	defined by macro c_QUOTED_ATTS
	bool quoted(t_cc);

//!	attribute of any type (value) creator.
//!	currently supported: t_cc, nob, t_num.
//!	recommended for t_cc type: maac
	template <class T>
	inline taa<T> maa(t_cc n, const T& v, bool q = false)
	{
		taa<T> t = { n, v, q };
		return t;
	}

	inline taan maa(t_cc n, t_num v, bool q = false)
	{
		taan t = { n, v, q };
		return t;
	}

//!	attribute of t_cc (value) creator.
	inline taac maac(t_cc n, t_cc v, bool q = false)
	{
		taac t = { n, v, q };
		return t;
	}

//!	auto quote attribute of any type (value) creator.
//!	currently supported: t_cc, nob, t_num.
	template <class T>
	inline taa<T> maq(t_cc n, const T& v)
	{
		taa<T> t = { n, v, quoted(n) };
		return t;
	}

//	attribute creator functions
	inline saa	mas(t_cc n) { saa t = { n }; return t; }
	inline laa	mal(t_cc c) { laa t = { c }; return t; }
	inline oaa	oa (t_cc c) { oaa t = { c }; return t; }
	extern eaa	ea;

//	standard attributes from string.
	inline taac align	(t_cc v) { return maac(c_align,	v		); }
	inline taac cl		(t_cc v) { return maac(c_class,	v		); }
	inline taac href  	(t_cc v) { return maac(c_href,	v, true	); }
	inline taac id		(t_cc v) { return maac(c_id,	v		); }
	inline taac lfor	(t_cc v) { return maac(c_for,	v		); }
	inline taac name	(t_cc v) { return maac(c_name,	v		); }
	inline taac src   	(t_cc v) { return maac(c_src,	v, true	); }
	inline taac title	(t_cc v) { return maac(c_title,	v, true	); }
	inline taac type	(t_cc v) { return maac(c_type,	v, true	); }
	inline taac value	(t_cc v) { return maac(c_value,	v, true	); }

//	nob attributes.
	inline taa<nob> cl   (const nob& v) { return maa<nob>(c_class, v); }
	inline taa<nob> id   (const nob& v) { return maa<nob>(c_id,    v); }
	inline taa<nob> lfor (const nob& v) { return maa<nob>(c_for,   v); }
	inline taa<nob> name (const nob& v) { return maa<nob>(c_name,  v); }

//	numerical attributes.
	inline taan colspan	  (t_num n) { return maa(c_colspan,	  n); }
	inline taan maxlength (t_num n) { return maa(c_maxlength, n); }
	inline taan size      (t_num n) { return maa(c_size,      n); }
	inline taan value     (t_num n) { return maa(c_value,     n); }

//	functor attributes.
	inline sfaa title (sfunc* sf) { sfaa t = { c_title, sf }; return t; }
	inline sfaa value (sfunc* sf) { sfaa t = { c_value, sf }; return t; }

//	single (non value) attributes.
	inline saa checked ()	{ return mas(c_checked); }
	inline saa readonly()	{ return mas(c_readonly); }
	inline saa selected()	{ return mas(c_selected); }
	inline saa multiple()	{ return mas(c_multiple); }

} // namespace ta (tag attribute)


//	============================================================
//	NEW table stream classes / functions
namespace tbl
{
//!	start a table.
	sfunc* tb();
//!	add a table row.
	sfunc* tr();
//!	add a table heading with autoclass.
	sfunc* th();
//!	add a table heading.
	sfunc* tha();
//!	add a table cell with outoclass.
	sfunc* td();
//!	add a table cell.
	sfunc* tda();
//!	end current table.
	sfunc* tbe();

//!	table standard begin function
//!	not quite standard conform but IE proof.
//!	you can re-define it here.
	inline void table_tbfunc()
	{
		t_num i = 0;
		dml << ta::maa(c_cellpadding, i) << ta::maa(c_cellspacing, i);
	}

} // namespace tbl

//	============================================================
//	convenience: other
//	the following functions are NOT (dml <<) streamable
//	because that would involve too mutch copying.

//!	convenience: form issue declarer - creates a hidden input
//!	with form issue name and individual issue value.
//! A page can contain many forms but only one form can be submitted.
//!	Therefore each form declares its indivdual issue.
//!	This saves parameter checks of content processors.
//!	@param c name of the issue
	void set_issue(t_cc c1, t_cc c2 = 0, t_cc c3 = 0);

//!	convenience: check if POSTed form is relevant issue.
//!	includes request of POSTed (is_post).
//!	CAUTION: changes value of cbf1
	bool is_issue(t_cc c1 = 0, t_cc c2 = 0, t_cc c3 = 0);

//!	javascript start.
	void start_js(bool = true);

//!	javascript file expansion.
	void jscript(t_cc);

	class cmapper;

//!	standard link start for internal content makers.
//!	@param cmq cmapper with query parameters
//!	@param cma cmapper with attribute parameters pointer
	void start_link(cmapper& cmq, cmapper* cma = 0, bool = true);

//!	standard link for internal content makers.
//!	@param cd link text
//!	@param cmq cmapper with query parameters
//!	@param cma cmapper with attribute parameters pointer
	void link(t_cc cd, cmapper& cmq, cmapper* cma = 0, bool = true);

//	============================================================
//	DCP SUPPORT
//	The following functions
//	should only be used by the dynamic content parser DCP.

//!	form begin including page information.
//!	This function should only be used by the dynamic content parser DCP.
//!	@param cma cmapper with attribute parameters
	void form(cmapper& cma);

//!	standard link.
//!	This function should only be used by the dynamic content parser DCP.
//!	@param cma cmapper with attribute parameters
	void link(cmapper& cma);

//!	parameter masking.
//!	used for link "params" attribute.
	void um_param(const s_cc&);



#endif // _H
