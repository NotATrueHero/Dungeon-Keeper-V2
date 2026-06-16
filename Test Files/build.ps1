$FTXUI = "..\FTXUI"
$dom = "$FTXUI\src\ftxui\dom"
$screen = "$FTXUI\src\ftxui\screen"

$sources = @(
    "$dom\automerge.cpp", "$dom\blink.cpp", "$dom\bold.cpp", "$dom\border.cpp",
    "$dom\box_helper.cpp", "$dom\canvas.cpp", "$dom\clear_under.cpp",
    "$dom\color.cpp", "$dom\composite_decorator.cpp", "$dom\dbox.cpp",
    "$dom\dim.cpp", "$dom\flex.cpp", "$dom\flexbox.cpp", "$dom\flexbox_config.cpp",
    "$dom\flexbox_helper.cpp", "$dom\focus.cpp", "$dom\frame.cpp", "$dom\gauge.cpp",
    "$dom\graph.cpp", "$dom\gridbox.cpp", "$dom\hbox.cpp", "$dom\hyperlink.cpp",
    "$dom\inverted.cpp", "$dom\italic.cpp", "$dom\linear_gradient.cpp",
    "$dom\node.cpp", "$dom\node_decorator.cpp", "$dom\paragraph.cpp",
    "$dom\reflect.cpp", "$dom\scroll_indicator.cpp", "$dom\selection.cpp",
    "$dom\selection_style.cpp", "$dom\separator.cpp", "$dom\size.cpp",
    "$dom\spinner.cpp", "$dom\strikethrough.cpp", "$dom\table.cpp",
    "$dom\text.cpp", "$dom\underlined.cpp", "$dom\underlined_double.cpp",
    "$dom\util.cpp", "$dom\vbox.cpp",
    "$screen\box.cpp", "$screen\color.cpp", "$screen\color_info.cpp",
    "$screen\screen.cpp", "$screen\string.cpp", "$screen\surface.cpp",
    "$screen\terminal.cpp"
)

g++ -std=c++17 -I "$FTXUI\include" -I "$FTXUI\src" @sources Navigation.cpp -o nav.exe
Write-Host "Build complete: nav.exe"
