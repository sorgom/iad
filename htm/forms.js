
function firstfocus()
{
	var f = window.document.forms[0];
	if (!f) return;
	var n = f.elements.length;
	if (!n) return;
	for (var i = 0; i < n; ++i) {
		var e = f.elements[i];
		if (
			(e.type == 'text' || e.type == 'password') &&
			(!e.readonly)
		){
			e.focus();
			e.setSelectionRange(0,0);	
			return;
		}
		if (e.tagName.toUpperCase() == 'SELECT') {
			e.focus();
			return;
		}
	}
}

