/*  moulin - offline wikipedia distro
    Copyright (C) 2006-2007, Kunnafoni Foundation and contributors
    
    Contributor(s):
        LinterWeb (France)
        Emmanuel Engelhart
        reg <reg@nurv.fr>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

/*
 * set of variables and functions to handle
 * the moulin interactions.
 * requierments:
 *  - global.js
 * 	- L class (logger.js)
 */

// will hold all the UI elem we want to keep in memory.  
var moulinUI = {};
moulinUI.macResizeInterval	= null;
moulinUI.findInPageDatas	= null;
moulinUI.textZoomFactor		= 1.3;
moulinUI.searchEngine		= 'encyclopedia';
moulinUI.winIsFullScreen	= false;
moulinUI.sideBarIsVisible	= false;

/* UI functions */

function UIInitElements () {
	// the <browser> containing webpages.
	moulinUI.browser			= UIGetBrowser ();
	moulinUI.searchField		= UIGetSearchField ();
	moulinUI.searchEngineButton	= UIGetSearchEngineButton ();
	moulinUI.sideBar			= UIGetSideBar ();
	moulinUI.bookmarkListBox	= UIGetBookmarkListBox ();
	moulinUI.notesTextBox		= UIGetNotesTextBox ();
}

function UIGetBrowser () {
	return document.getElementById ('content-browser');
}

function UIGetSearchField () {
	return document.getElementById ('searchbar');
}

function UIGetSideBar () {
	return document.getElementById ('moulin-sidebar');
}

function UIGetSearchEngineButton () {
	return document.getElementById ('search');
}

function UIGetBookmarkListBox () {
	return document.getElementById ('bookmarkListBox');
}

function UIGetNotesTextBox () {
	return document.getElementById ('notesTextBox');
}

/*
 * on the Mac, page display is buggy at launch
 * resizing the main windows refreshes it correctly
 * so this function is launched (with delay) on init.
 */
function UIResizeWindows () {
	window.fullScreen = 'true';
	try {
		clearInterval (moulinUI.macResizeInterval);
	} catch (e) {
		L.error ("Can't clear interval from UIResizeWindows:" + e.toString ());
	}
}

/*
 * Tells the browser to display it's homepage
 * that the value of the `homepage` attribute, not the initial src.
 */
function UINavGoHome () {
	try {
		moulinUI.browser.goHome ();
	} catch (e) {
		L.info ("can't go home, not set ?:" + e.toString ());
	}
}

/*
 * Launched by toolbar Back button ; forwards to browser
 */
function UINavBack () {
	try {
		moulinUI.browser.stop ();
		moulinUI.browser.goBack ();
		return true;
	} catch (e) {
		return false;
	}
}

/*
 * Launched by toolbar Forward button ; forwards to browser
 */
function UINavForward () {
	try {
		moulinUI.browser.stop ();
		moulinUI.browser.goForward ();
		return true;
	} catch (e) {
		return false;
	}
}

/*
 * Opens the Find in Page dialog.
 */
function UIFindInPage () {
	if (!moulinUI.findInPageDatas) {
		moulinUI.findInPageDatas = new nsFindInstData ();
		moulinUI.findInPageDatas.browser = moulinUI.browser;
	}

	var lastSearch	= ""; // should set it as current selection ; how ?
	var bLastWord	= lastSearch.lastIndexOf (" ", lastSearch.length);
	moulinUI.findInPageDatas.webBrowserFind.searchString = lastSearch
	.substring (bLastWord+1, lastSearch.length);
	findInPage (moulinUI.findInPageDatas);
}

/*
 * Opens the print dialog and print current page.
 */
function UIPrintPage () {
	try {
		return PrintUtils.print ();
		//PrintUtils.printPreview ();
		//PrintUtils.showPageSetup ();
	} catch (e) {
		L.error ("can't call print dialog:" + e.toString ());
		return false;
	}
}

/*
 * Copies in-browser selection to browser pasteboard
 */
function UICopySelection () {
	try {
		moulinUI.browser.contentViewerEdit.copySelection ();
	} catch (e) {
		L.error ("can't copy selection:" + e.toString ());
	}
}

/*
 * selectAll in browser
 */
function UISelectAll () {
	try {
		moulinUI.browser.contentViewerEdit.selectAll ();
	} catch (e) {
		L.error ("can't select all:" + e.toString ());
	}
}

/*
 * Increase all font sizes by factor on page
 */
function UITextBigger () {
	try {
		moulinUI.browser.markupDocumentViewer.textZoom *= moulinUI.textZoomFactor;
	} catch (e) {
		L.error ("can't make text bigger:" + e.toString ());
	}
}

/*
 * Decrease all font sizes by factor on page
 */
function UITextSmaller () {
	try {
		moulinUI.browser.markupDocumentViewer.textZoom /= moulinUI.textZoomFactor;
	} catch (e) {
		L.error ("can't make text smaller:" + e.toString ());
	}
}

function UIFocusOnSearch () {
	try {
		moulinUI.searchField.focus ();
	} catch (e) {
		L.error ("can't focus on search field");
	}
}

/*
 * Opens URL with default browser/handler
 */
function UIOpenExternalURI (url) {
	try { 
		var extps = Components
		.classes ["@mozilla.org/uriloader/external-protocol-service;1"]
		.getService ()
		.QueryInterface (Components.interfaces.nsIExternalProtocolService);
	
		var ioService = Components.classes ["@mozilla.org/network/io-service;1"]
		.getService ().QueryInterface (Components.interfaces.nsIIOService);
	
		extps.loadUrl (ioService.newURI (url, null, null));
	} catch (e) {
		L.error ("can't open url with default browser:" + e.toString ());
	}
}

/*
 * helper to load any URI on browser
 */
function UINavGoTo (url) {
	try {
		moulinUI.browser.loadURI (url, null, null);
	} catch (e) {
		L.error ("can't load URL:" + e.toString ());
	}
}

/*
 * Called by toolbar command : launch a search based on field's value.
 */
function UINavSearch () {
	searchString = moulinUI.searchField.value;
	try {
		UINavGoTo ("moulin-search://" + moulinUI.searchEngine + "/" + DEFAULT_LANGUAGE_DATA + "/" + searchString);
	} catch (e) {
		L.error (e.toString ());
	}
}

/*
 * Enable/Disable fullscreen mode. Acts as window maximizer on mac.
 */
function UIToggleFullScreen () {
	if (moulinUI.winIsFullScreen) {
		window.fullScreen = false;
		moulinUI.winIsFullScreen = false;
	} else { 
		window.fullScreen = true;
		moulinUI.winIsFullScreen = true;
	}
}

/*
 * Display/Hide the Bookmarks&Notes sidebar.
 */
function UIToggleSideBar () {
	if (moulinUI.sideBarIsVisible) {
		moulinUI.sideBar.hidden = true;
		moulinUI.sideBarIsVisible = false;
	} else {
		moulinUI.sideBar.hidden = false;
		moulinUI.sideBarIsVisible = true;
	}
}

/*
 * Called by click on a search engine in the SE list.
 * Switch the SE icon on bar and modify search target.
 */
function UIChangeSearchEngine (project) {
	try {
		moulinUI.searchEngineButton
		.setAttribute('image', 'chrome://moulin/locale/projects/'+project+'_list.png');
		moulinUI.searchEngine = project;
	} catch (e) {
		L.error ("can't change search engine:" + e.toString ());
	}
}


