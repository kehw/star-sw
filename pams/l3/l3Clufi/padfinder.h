// here you find for each row up to 3 i960(=mezz cards) which
// the pads in this row belong to 

static struct PADFINDER {
    int rdo;    // receiver board number
    int mezz;   // mezzanine card number
    int minpad; // low pad
    int maxpad; // max pad
} padfinder[45][3] = 

{
{{1, 1, 1, 88} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{1, 1, 1, 96} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{1, 1, 1, 4} ,
{1, 1, 101, 104} ,
{1, 2, 5, 100} },

{{1, 2, 1, 112} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{1, 2, 4, 115} ,
{1, 3, 1, 3} ,
{1, 3, 116, 118} },

{{1, 3, 1, 126} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{1, 3, 1, 134} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{1, 3, 13, 130} ,
{2, 3, 1, 12} ,
{2, 3, 131, 142} },

{{2, 3, 1, 150} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{2, 3, 1, 158} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{2, 2, 6, 161} ,
{2, 3, 1, 5} ,
{2, 3, 162, 166} },

{{2, 1, 3, 172} ,
{2, 2, 1, 2} ,
{2, 2, 173, 174} },

{{2, 1, 1, 182} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{3, 2, 1, 1} ,
{3, 3, 2, 98} ,
{0, 0, 0, 0} },

{{3, 2, 1, 2} ,
{3, 3, 3, 100} ,
{0, 0, 0, 0} },

{{3, 2, 1, 43} ,
{3, 3, 44, 102} ,
{0, 0, 0, 0} },

{{3, 2, 1, 104} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{3, 2, 1, 106} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{3, 1, 1, 93} ,
{3, 2, 94, 106} ,
{0, 0, 0, 0} },

{{3, 1, 1, 108} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{3, 1, 1, 110} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{4, 3, 1, 112} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{4, 3, 1, 112} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{4, 2, 1, 49} ,
{4, 2, 114, 114} ,
{4, 3, 50, 113} },

{{4, 2, 1, 116} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{4, 2, 1, 118} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{4, 1, 1, 92} ,
{4, 2, 93, 120} ,
{0, 0, 0, 0} },

{{4, 1, 1, 122} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{4, 1, 1, 122} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{5, 3, 1, 124} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{5, 3, 1, 126} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{5, 2, 1, 48} ,
{5, 3, 49, 128} ,
{0, 0, 0, 0} },

{{5, 2, 1, 128} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{5, 1, 1, 1} ,
{5, 1, 130, 130} ,
{5, 2, 2, 129} },

{{5, 1, 1, 82} ,
{5, 1, 131, 132} ,
{5, 2, 83, 130} },

{{5, 1, 1, 134} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{5, 1, 1, 136} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{6, 3, 1, 138} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{6, 3, 1, 138} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{6, 2, 1, 46} ,
{6, 3, 47, 140} ,
{0, 0, 0, 0} },

{{6, 2, 1, 142} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{6, 2, 1, 144} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{6, 1, 1, 96} ,
{6, 2, 97, 144} ,
{0, 0, 0, 0} },

{{6, 1, 1, 144} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} },

{{6, 1, 1, 144} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} }};
