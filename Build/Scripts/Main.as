
#include "Tokens.as"

void main()
{
    camera_proj_mat(90.0f, 0.1f, 100.0f);
    camera_pos(0.0f, 0.0f, 1.0f);

    kbd_on_press(k_escape, function(){
        exit();
    });
    /* kbd_on_press(k_n1, function(){
        option_draw_point();
    }); */
    kbd_on_press(k_n1, function(){
        option_draw_lines();
    });
    kbd_on_press(k_n2, function(){
        option_draw_fill();
    });
    
    /* bool b = true;
    add_runtime_cb(function(){
        if (kbd_key(k_f))
        {
            b = !b;
        }
    }); */

}
