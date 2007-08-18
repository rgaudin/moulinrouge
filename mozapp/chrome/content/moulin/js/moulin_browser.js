/*  moulin - offline wikipedia distro
    Copyright (C) 2006-2007, Kunnafoni Foundation and contributors
    
    Contributor(s):
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
 * functions for the browser
 * requierments:
 *  - global.js
 * 	- L class (logger.js)
 *  - moulin_ui.js
 */

var moulinNFO 			= {};
moulinNFO.OS			= GuessOS ();
moulinNFO.applicationFD	= GetApplicationFolder ();

const nsIWebProgress = Components.interfaces.nsIWebProgress;
const nsIWebProgressListener = Components.interfaces.nsIWebProgressListener;


function GetApplicationFolder () {
	try {
		return Components.classes ["@mozilla.org/file/directory_service;1"]
		.getService (Components.interfaces.nsIProperties)
		.get ("resource:app", Components.interfaces.nsIFile);
	} catch (e) {
		L.error ("can't get app folder:" + e.toString ());
		return false;
	}
}
/*
 * return [win|mac|unix] depending on the platform running.
 * usefull to trick mac specificities.
 */
function GuessOS () {
    var runtime = Components.classes ["@mozilla.org/xre/app-info;1"]
    .getService(Components.interfaces.nsIXULRuntime);
    var tmp = runtime.OS;
	var platform = {};
    
    if (tmp.match(/^win/i)) { // send condoleances
        platform.type = "win";
        platform.string = tmp;
        return platform;
    }
    if (tmp.match(/^darwin/i)) { // send freedom speech
        platform.type = "mac";
        platform.string = tmp;
        return platform;
    } else { // send drivers
        platform.type = "unix";
        platform.string = tmp;
    }
    return platform;
}

/*
 * registered by listener on every mouse scroll. changing text size if ctrl.
 */
function MouseScrollEventHandler (aEvent) {
	if (aEvent.ctrlKey) {
		if (aEvent.detail > 0)
			UITextBigger ();
    	if (aEvent.detail < 0)
    		UITextSmaller ();
    	aEvent.preventDefault ();
    	aEvent.stopPropagation ();
	}
}

/*
 * registered by listener on every activate action (click,etc).
 */
function ActivateEventHandler (aEvent)
{
	var link = aEvent.target;
	L.info ('LINKK: '+link.href);

	if (link instanceof HTMLAnchorElement) {
	
		// this is necessary to be able to go to chrome:// from moulin://
		if ( (link.href.indexOf ("chrome://",0) ==0) || (link.href.indexOf ("moulin://",0) == 0) || (link.href.indexOf ("moulin-search://",0) == 0)) {
			UINavGoTo (link.href);
			aEvent.preventDefault ();
			aEvent.stopPropagation ();
		}
		
		// we open http and https URLs on the default browser.	
    	if ( (link.href.indexOf ("http://",0) == 0) || (link.href.indexOf ("https://",0) == 0)) {
			UIOpenExternalURI (link.href);
			aEvent.preventDefault ();
			aEvent.stopPropagation ();
    	}
	}
}

function RemoveListener (aEvent) {
  //aEvent.target.ownerDocument.removeEventListener("mouseover", MouseOver, true);
  aEvent.target.ownerDocument.removeEventListener ("DOMMouseScroll", ActivateEventHandler, true);
  aEvent.target.ownerDocument.removeEventListener ("DOMActivate", ActivateEventHandler, true);
  aEvent.target.ownerDocument.removeEventListener ("unload", RemoveListener, false);
  UISaveCurrentNote ();
}

const UIBrowserProgressListener = {
  
	onStateChange: function osc (aWP, aRequest, aStateFlags, aStatus) {

		if (aStateFlags & nsIWebProgressListener.STATE_STOP) {
			//L.error ("STATE_STOP catched");
			var myDocument = aWP.DOMWindow.document;
      
			//myDocument.addEventListener ("mouseover", MouseOver, true);
			//myDocument.addEventListener ("mouseout", MouseOut, true);
			myDocument.addEventListener ("DOMMouseScroll", MouseScrollEventHandler, true);
			myDocument.addEventListener ("DOMActivate", ActivateEventHandler, true);
			myDocument.addEventListener ("unload", RemoveListener, false);
		}
	},
  
	onLocationChange: function olc (wp,request,location) {
		//L.info ("onLocationChange: "+location.spec.toString ());
		UILoadNoteForUrl (location.spec);
	},
	
	QueryInterface: function qi (aIID) {
    	if (aIID.equals(nsIWebProgressListener) ||
			aIID.equals(Components.interfaces.nsISupports) ||
			aIID.equals(Components.interfaces.nsISupportsWeakReference)) {
			return this;
		}
    	throw Components.results.NS_ERROR_NO_INTERFACE;
	}
};

/*
 * Called by the onload event of the <browser>
 * Set up default stuffs
 */
function initializeBrowser () {

	// set UI elem variables
	UIInitElements ();

	// Guess and set homepage
	var docrootFD 		= moulinNFO.applicationFD.clone ();
	var todocrootpath	= "chrome/locale/" + DEFAULT_LANGUAGE_UI + "/moulin/docroot";
	for each (var i in todocrootpath.split ("/")) { docrootFD.append (i); }	
	var docroot	= docrootFD.path;

	// register WebProgress listener
	var dls = Components.classes ["@mozilla.org/docloaderservice;1"]
	.getService (nsIWebProgress);
	dls.addProgressListener (UIBrowserProgressListener,
							nsIWebProgress.NOTIFY_LOCATION |
							nsIWebProgress.NOTIFY_STATUS |
							nsIWebProgress.NOTIFY_STATE_DOCUMENT);
                          
	moulinUI.browser.setAttribute ('homepage', "file:///" + docroot + "/home.html");
	
	// Load Notes for homepage.
	//loadNotes(null);
	InitializeNotes ();

	// resize main window on mac to 'refresh' display since it's buggy.
	if (moulinNFO.OS.type == 'mac')	{
		moulinUI.macResizeInterval = setInterval (UIResizeWindows, 1000); //0.5s
	}
	
	// Load Default Bookmarks
	InitializeBookmarks ();
}


