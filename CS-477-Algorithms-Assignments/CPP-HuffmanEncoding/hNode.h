class hNode{
    public:
        hNode();
        hNode(float);
        float w;
        hNode *p,*l,*r;
        char symbol;
        hNode& operator = ( const hNode& );
        friend bool operator< ( hNode& a , hNode&b );

};

hNode::hNode(){
    w = 0;
    p = l = r = NULL; 
}

hNode::hNode(float weight){
    w = weight;
    p = l = r = NULL;
    symbol = NULL;
}

hNode& hNode::operator = ( const hNode &rhs ){
    if( this != &rhs ){
        w = rhs.w;
        p = rhs.p;
        l = rhs.l;
        r = rhs.r;
        symbol = rhs.symbol;
    }
    return *this;
}

bool operator< ( hNode &a, hNode &b ){
    return ( a.w < b.w );
}
