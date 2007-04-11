namespace WikiMoulin {

	using System;
	using System.Collections.Generic;
    
    class WikiLangMap {
    
        static Dictionary<string, int> s_ns = new Dictionary<string, int> ();
        
        public Dictionary<string, int> Ns { get{return s_ns;} }
        
        static WikiLangMap () {
			s_ns.Add ("Media", -2);
			s_ns.Add ("Speciala", -1);
			s_ns.Add ("", 0);
			s_ns.Add ("Diskuto", 1);
			s_ns.Add ("Vikipediisto", 2);
			s_ns.Add ("Vikipediista diskuto", 3);
			s_ns.Add ("Vikipedio", 4);
			s_ns.Add ("Vikipedia diskuto", 5);
			s_ns.Add ("Dosiero", 6);
			s_ns.Add ("Dosiera diskuto", 7);
			s_ns.Add ("MediaWiki", 8) ;
			s_ns.Add ("MediaWiki diskuto", 9);
			s_ns.Add ("ablono", 10);
			s_ns.Add ("ablona diskuto", 11);
			s_ns.Add ("Helpo", 12);
			s_ns.Add ("Helpa diskuto", 13);
			s_ns.Add ("Kategorio", 14);
			s_ns.Add ("Kategoria diskuto", 15);
			s_ns.Add ("Portalo", 100);
			s_ns.Add ("Portala diskuto", 101);
        }
    }
}
