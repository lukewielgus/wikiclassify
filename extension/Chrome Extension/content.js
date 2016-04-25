/* showHide();

function showHide() {
    var el = document.getElementById("bodyContent");
    if( el && el.style.display == 'none')
        el.style.display = 'block';
    else if( el )
        el.style.display = 'none';
} */

/* chrome.tabs.query({ active: true, currentWindow: true }, function (tab) {
	title = tab.title;
	
	document.write(title);
}); */

function displayClassification() {
	var title = 'Classification';
	document.write(title);
}
 
displayClassification();