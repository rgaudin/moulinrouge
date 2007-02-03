namespace WikiMoulin {

	using System;
	using System.Collections.Generic;
    
    class WikiLangMap {
    
        static Dictionary<string, int> s_ns = new Dictionary<string, int> ();
        
        public Dictionary<string, int> Ns { get{return s_ns;} }
        
        static WikiLangMap () {
			s_ns.Add ("Media", -2);
			s_ns.Add ("Speciale", -1);
			s_ns.Add ("", 0);
			s_ns.Add ("Discussione", 1);
			s_ns.Add ("Utente", 2);
			s_ns.Add ("Discussioni utente", 3);
			s_ns.Add ("Wikipedia", 4);
			s_ns.Add ("Discussioni Wikipedia", 5);
			s_ns.Add ("Immagine", 6);
			s_ns.Add ("Discussioni immagine", 7);
			s_ns.Add ("MediaWiki", 8) ;
			s_ns.Add ("Discussioni MediaWiki", 9);
			s_ns.Add ("Template", 10);
			s_ns.Add ("Discussioni template", 11);
			s_ns.Add ("Aiuto", 12);
			s_ns.Add ("Discussioni Aiuto", 13);
			s_ns.Add ("Categoria", 14);
			s_ns.Add ("Discussioni categoria", 15);
			s_ns.Add ("Portale", 100);
			s_ns.Add ("Discussioni portale", 101);
			s_ns.Add ("Progetto", 102);
			s_ns.Add ("Discussioni progetto", 103);
        }
    }
}