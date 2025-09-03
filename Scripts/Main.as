void main()
{
    global_func();
    global_func();
    print_num(3);
    print_num(4);
    print_num(2025);

    set_callback(function(){
        global_func();
        print_num(40);
    });

    /* kbd_on_press(49, 
        function(){
            option_draw_lines();
        }
    ); */


}
