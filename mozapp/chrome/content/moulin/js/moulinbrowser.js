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

// Factor by which font is magnified by pressing ctrl+'+'
var zoomFactor = 1.3;
// Home url
var homeUrl = "test2.html";
// Absolute path to the html directory
var rootPath;
// structure for the find in page dialog
var findInstData=null;
//var moulinComp = Components.classes["@kunnafoni.org/cpp_nsMoulin;1"].createInstance(Components.interfaces.nsIMoulin);
var baseDocrootFD = Components.classes["@mozilla.org/file/directory_service;1"].getService(Components.interfaces.nsIProperties).get("resource:app", Components.interfaces.nsIFile);

function getBrowser() {

  return document.getElementById("content-browser");  
}

const nsIWebProgress = Components.interfaces.nsIWebProgress;
const nsIWebProgressListener = Components.interfaces.nsIWebProgressListener;

function MouseScroll(aEvent) {

  if ( aEvent.ctrlKey ) {
    if ( aEvent.detail > 0 ) zoomin();
    if ( aEvent.detail < 0 ) zoomout();
    aEvent.preventDefault();
    aEvent.stopPropagation();
  }
}

function Activate(aEvent)
{
  var link = aEvent.target;

  if (link instanceof HTMLAnchorElement) {

    if (link.href.indexOf("moulin://",0)==0) {
		getBrowser().loadURI(link.href, null, null);
		aEvent.preventDefault();
		aEvent.stopPropagation();
    }

    if ((link.href.indexOf("http://",0)==0)||(link.href.indexOf("https://",0)==0)) {

      // We don't want to open external links in this process: do so in the
      // default browser.
      var ios = Components.classes["@mozilla.org/network/io-service;1"].
        getService(Components.interfaces.nsIIOService);

      var resolvedURI = ios.newURI(link.href, null, null);

      var extps = Components.
        classes["@mozilla.org/uriloader/external-protocol-service;1"].
        getService(Components.interfaces.nsIExternalProtocolService);

      extps.loadURI(resolvedURI, null);
      aEvent.preventDefault();
      aEvent.stopPropagation();
    }
  }
}

function RemoveListener(aEvent) {
  aEvent.target.ownerDocument.removeEventListener("mouseover", MouseOver, true);
  aEvent.target.ownerDocument.removeEventListener("DOMMouseScroll", Activate, true);
  aEvent.target.ownerDocument.removeEventListener("DOMActivate", Activate, true);
  aEvent.target.ownerDocument.removeEventListener("unload", RemoveListener, false);
}

const listener = {
  
  onStateChange: function osc(aWP, aRequest, aStateFlags, aStatus) {

    if (aStateFlags & nsIWebProgressListener.STATE_STOP) {
      Components.utils.reportError("STATE_STOP");

	//aWP.DOMWindow.title = getBrowser().contentTitle;

      var myDocument = aWP.DOMWindow.document;
      myDocument.addEventListener("mouseover", MouseOver, true);
      myDocument.addEventListener("mouseout", MouseOut, true);
      myDocument.addEventListener("DOMMouseScroll", MouseScroll, true);
      myDocument.addEventListener("DOMActivate", Activate, true);
      myDocument.addEventListener("onchange", Transition, true);
      myDocument.addEventListener("unload", RemoveListener, false);
    }
  },
  onLocationChange: function olc(wp,request,location) {
  },
  QueryInterface: function qi(aIID) {
    if (aIID.equals(nsIWebProgressListener) ||
        aIID.equals(Components.interfaces.nsISupports) ||
        aIID.equals(Components.interfaces.nsISupportsWeakReference)) {
      return this;
    }
    throw Components.results.NS_ERROR_NO_INTERFACE;
  },
};

// Called at startup
// listener for catching special links
function initRoot() {


  var docrootFD = baseDocrootFD.clone();
  var todocrootpath = "datas/"+DEFAULT_LANGUAGE_DATA+"/docroot";
  for each (var i in todocrootpath.split("/")) { docrootFD.append(i); }	
  rootPath =  docrootFD.path;

  var dls = Components.classes["@mozilla.org/docloaderservice;1"].getService(Components.interfaces.nsIWebProgress);
  dls.addProgressListener(listener,
                          nsIWebProgress.NOTIFY_LOCATION_DOCUMENT |
                          nsIWebProgress.NOTIFY_STATE |
                          nsIWebProgress.NOTIFY_STATE_DOCUMENT);
                          
  getBrowser().setAttribute( "homepage", "file:///"+rootPath+"/"+homeUrl );
}

function back() {
	try{
		var browser = getBrowser();
		browser.stop();
		browser.goBack();
	}catch(e){
		return false;
		dump(e);
	}
	return true;
}

function forward() {
	try{
		var browser = getBrowser();
		browser.stop();
		browser.goForward();
	}catch(e){
		return false;
		dump(e);
	}
	return true;
}

// open the "find in page" dialog
function findin() {
  if ( !findInstData ) {
    findInstData = new nsFindInstData();
    findInstData.browser = getBrowser();
  }
  var lastSearch = ""; // should set it as current selection
  var bLastWord = lastSearch.lastIndexOf( " ", lastSearch.length );
  findInstData.webBrowserFind.searchString = lastSearch.substring( bLastWord+1, lastSearch.length );
  findInPage( findInstData );
}

// open the "print" dialog
function print(){
      //searchPopupClose();
	try{
		var tt = PrintUtils.print();
		
		//apercu avant impression ==> printPreview();
		//page de modification des marges ==> showPageSetup();
		//getWebBrowserPrint;
	}catch(e){
		return false;
		dump(e);
	}
}

function copy() {

  getBrowser().contentViewerEdit.copySelection();
}

function selectall() {

  getBrowser().contentViewerEdit.selectAll();
}

function gohome() {

  getBrowser().goHome();
}

function openexternal() {
  
	var extps = Components.classes["@mozilla.org/uriloader/external-protocol-service;1"].getService(Components.interfaces.nsIExternalProtocolService);
	extps.loadUrl(getBrowser().currentURI );
}

function zoomin() {
	getBrowser().markupDocumentViewer.textZoom *= zoomFactor;
}

function zoomout() {

  getBrowser().markupDocumentViewer.textZoom /= zoomFactor;
}

function openexternalurl(url) {

  var extps = Components.classes["@mozilla.org/uriloader/external-protocol-service;1"].getService().QueryInterface(Components.interfaces.nsIExternalProtocolService);
  var ioService = Components.classes["@mozilla.org/network/io-service;1"].getService().QueryInterface(Components.interfaces.nsIIOService);
  extps.loadUrl(ioService.newURI(url, null, null) );
}

function setbrowser() {

 var prefs = Components.classes["@mozilla.org/preferences-service;1"].getService().QueryInterface(Components.interfaces.nsIPrefBranch);
 var name = prefs.getCharPref("network.protocol-handler.app.http" );
 var newname = prompt('Enter the path to your external browser',name);
 if ( newname != null ) 
   prefs.setCharPref("network.protocol-handler.app.http", newname );
}

