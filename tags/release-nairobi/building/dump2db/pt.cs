namespace WikiMoulin {

	using System;
	using System.Collections.Generic;
    
    class WikiLangMap {
    
        static Dictionary<string, int> s_ns = new Dictionary<string, int> ();
        
        public Dictionary<string, int> Ns { get{return s_ns;} }
        
        static WikiLangMap () {
			s_ns.Add ("Media", -2);
			s_ns.Add ("Especial", -1);
			s_ns.Add ("", 0);
			s_ns.Add ("Discussão", 1);
			s_ns.Add ("Usuário", 2);
			s_ns.Add ("Usuário Discussão", 3);
			s_ns.Add ("Wikipedia", 4);
			s_ns.Add ("Wikipedia  Discussão", 5);
			s_ns.Add ("Imagem", 6);
			s_ns.Add ("Imagem Discussão", 7);
			s_ns.Add ("MediaWiki", 8) ;
			s_ns.Add ("MediaWiki Discussão", 9);
			s_ns.Add ("Predefinição", 10);
			s_ns.Add ("Predefinição Discussão", 11);
			s_ns.Add ("Ajuda", 12);
			s_ns.Add ("Ajuda Discussão", 13);
			s_ns.Add ("Categoria", 14);
			s_ns.Add ("Categoria Discussão", 15);
			s_ns.Add ("Portal", 100);
			s_ns.Add ("Portal Discussão", 101);
        }
    }
}