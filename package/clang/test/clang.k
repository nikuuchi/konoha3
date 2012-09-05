K.import("konoha");
K.import("clang");

void visit_node(Cursor node) {
	System.p(node.getKindName());
	System.p(node.getSpelling());
//	Array[Cursor] ac = node.getChildren();
//	int i = 0;
//	while(i < ac.size) {
//		visit_node(ac[i]);
//		i++;
//	}
}

Index idx = new Index();
TranslationUnit cur = idx.parse("test.c");
Cursor node = cur.getCursor();
visit_node(node);
