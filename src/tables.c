#include "tables.h"

/* char tables are inspired by user psef15 @ SS/14 */

bool check_method_char(char character) {
    static const unsigned char method_chartable[128] = {
            /* nul soh stx etx eot enq ack bel */
                0,  0,  0,  0,  0,  0,  0,  0,
            /* bs  ht  lf  vt  ff  cr  so  si  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /* dle dc1 dc2 dc3 dc4 nak syn etb */
                0,  0,  0,  0,  0,  0,  0,  0,
            /* can em  sub esc fs  gs  rs  us  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /* sp   !   "   #   $   %   &   '  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /*  (   )   *   +   ,   -   .   /  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /*  0   1   2   3   4   5   6   7  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /*  8   9   :   ;   <   =   >   ?  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /*  @   A   B   C   D   E   F   G  */
                0,  1,  1,  1,  1,  1,  1,  1,
            /*  H   I   J   K   L   M   N   O  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  P   Q   R   S   T   U   V   W  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  X   Y   Z   [   \   ]   ^   _  */
                1,  1,  1,  0,  0,  0,  0,  0,
            /*  `   a   b   c   d   e   f   g  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /*  h   i   j   k   l   m   n   o  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /*  p   q   r   s   t   u   v   w  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /*  x   y   z   {   |   }   ~  del */
                0,  0,  0,  0,  0,  0,  0,  0
    };
    if (character < 0 || character > 127) {
        return false;
    }
    return method_chartable[(unsigned) character] == 1 ? true : false;
}

bool check_version_char(char character) {
    static const unsigned char uri_chartable[128] = {
            /* nul soh stx etx eot enq ack bel */
                0,  0,  0,  0,  0,  0,  0,  0,
            /* bs  ht  lf  vt  ff  cr  so  si  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /* dle dc1 dc2 dc3 dc4 nak syn etb */
                0,  0,  0,  0,  0,  0,  0,  0,
            /* can em  sub esc fs  gs  rs  us  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /* sp   !   "   #   $   %   &   '  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /*  (   )   *   +   ,   -   .   /  */
                0,  0,  0,  0,  0,  0,  1,  1,
            /*  0   1   2   3   4   5   6   7  */
                1,  1,  0,  0,  0,  0,  0,  0,
            /*  8   9   :   ;   <   =   >   ?  */
                0,  1,  0,  0,  0,  0,  0,  0,
            /*  @   A   B   C   D   E   F   G  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /*  H   I   J   K   L   M   N   O  */
                1,  0,  0,  0,  0,  0,  0,  0,
            /*  P   Q   R   S   T   U   V   W  */
                1,  0,  0,  0,  1,  0,  0,  0,
            /*  X   Y   Z   [   \   ]   ^   _  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /*  `   a   b   c   d   e   f   g  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /*  h   i   j   k   l   m   n   o  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /*  p   q   r   s   t   u   v   w  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /*  x   y   z   {   |   }   ~  del */
                0,  0,  0,  0,  0,  0,  0,  0
    };
    if (character < 0 || character > 127) {
        return false;
    }
    return uri_chartable[(unsigned) character] == 1 ? true : false;
}

bool check_uri_char(char character) {
    static const unsigned char uri_chartable[128] = {
            /* nul soh stx etx eot enq ack bel */
                0,  0,  0,  0,  0,  0,  0,  0,
            /* bs  ht  lf  vt  ff  cr  so  si  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /* dle dc1 dc2 dc3 dc4 nak syn etb */
                0,  0,  0,  0,  0,  0,  0,  0,
            /* can em  sub esc fs  gs  rs  us  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /* sp   !   "   #   $   %   &   '  */
                0,  1,  1,  1,  1,  1,  1,  1,
            /*  (   )   *   +   ,   -   .   /  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  0   1   2   3   4   5   6   7  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  8   9   :   ;   <   =   >   ?  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  @   A   B   C   D   E   F   G  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  H   I   J   K   L   M   N   O  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  P   Q   R   S   T   U   V   W  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  X   Y   Z   [   \   ]   ^   _  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  `   a   b   c   d   e   f   g  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  h   i   j   k   l   m   n   o  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  p   q   r   s   t   u   v   w  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  x   y   z   {   |   }   ~  del */
                1,  1,  1,  1,  1,  1,  1,  0
    };
    if (character < 0 || character > 127) {
        return false;
    }
    return uri_chartable[(unsigned) character] == 1 ? true : false;
}

/* RFC 1945 2.2 */
bool check_header_name_char(char character) {
    static const unsigned char header_name_chartable[128] = {
            /* nul soh stx etx eot enq ack bel */
                0,  0,  0,  0,  0,  0,  0,  0,
            /* bs  ht  lf  vt  ff  cr  so  si  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /* dle dc1 dc2 dc3 dc4 nak syn etb */
                0,  0,  0,  0,  0,  0,  0,  0,
            /* can em  sub esc fs  gs  rs  us  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /* sp   !   "   #   $   %   &   '  */
                0,  1,  0,  1,  1,  1,  1,  1,
            /*  (   )   *   +   ,   -   .   /  */
                0,  0,  1,  1,  0,  1,  1,  0,
            /*  0   1   2   3   4   5   6   7  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  8   9   :   ;   <   =   >   ?  */
                1,  1,  0,  0,  0,  0,  0,  0,
            /*  @   A   B   C   D   E   F   G  */
                0,  1,  1,  1,  1,  1,  1,  1,
            /*  H   I   J   K   L   M   N   O  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  P   Q   R   S   T   U   V   W  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  X   Y   Z   [   \   ]   ^   _  */
                1,  1,  1,  0,  0,  0,  1,  1,
            /*  `   a   b   c   d   e   f   g  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  h   i   j   k   l   m   n   o  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  p   q   r   s   t   u   v   w  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  x   y   z   {   |   }   ~  del */
                1,  1,  1,  0,  1,  0,  1,  0
    };
    if (character < 0 || character > 127) {
        return false;
    }
    return header_name_chartable[(unsigned) character] == 1 ? true : false;
}


/* RFC 1945 2.2 */
bool check_header_value_char(char character) {

    /* everything that is not ASCII is allowed */
    if (character < 0 || character > 127)
        return true;

    /* no CTLs (except HT) allowed */
    if (character != '\t' && character < ' ')
        return false;

    /* DEL is not allowed */
    if (character == 127)
        return false;

    /* everything else is allowed */
    return true;
}

bool check_auth_header_base64_char(char character) {
    static const unsigned char auth_header_base64_chartable[128] = {
            /* nul soh stx etx eot enq ack bel */
                0,  0,  0,  0,  0,  0,  0,  0,
            /* bs  ht  lf  vt  ff  cr  so  si  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /* dle dc1 dc2 dc3 dc4 nak syn etb */
                0,  0,  0,  0,  0,  0,  0,  0,
            /* can em  sub esc fs  gs  rs  us  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /* sp   !   "   #   $   %   &   '  */
                0,  0,  0,  0,  0,  0,  0,  0,
            /*  (   )   *   +   ,   -   .   /  */
                0,  0,  0,  1,  0,  0,  0,  1,
            /*  0   1   2   3   4   5   6   7  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  8   9   :   ;   <   =   >   ?  */
                1,  1,  0,  0,  0,  1,  0,  0,
            /*  @   A   B   C   D   E   F   G  */
                0,  1,  1,  1,  1,  1,  1,  1,
            /*  H   I   J   K   L   M   N   O  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  P   Q   R   S   T   U   V   W  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  X   Y   Z   [   \   ]   ^   _  */
                1,  1,  1,  0,  0,  0,  0,  0,
            /*  `   a   b   c   d   e   f   g  */
                0,  1,  1,  1,  1,  1,  1,  1,
            /*  h   i   j   k   l   m   n   o  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  p   q   r   s   t   u   v   w  */
                1,  1,  1,  1,  1,  1,  1,  1,
            /*  x   y   z   {   |   }   ~  del */
                1,  1,  1,  0,  0,  0,  0,  0
    };
    if (character < 0 || character > 127) {
        return false;
    }
    return auth_header_base64_chartable[(unsigned) character] == 1 ? true : false;
}
