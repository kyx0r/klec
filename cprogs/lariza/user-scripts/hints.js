// This is NOT a core component of lariza, but an optional user script.
// Please refer to lariza.usage(1) for more information on user scripts.

// Press "f" (open link in current window) or "F" (open in new window)
// to activate link hints. After typing the characters for one of them,
// press Enter to confirm. Press Escape to abort.
//
// This is an "80% solution". It works for many web sites, but has
// flaws. For more background on this topic, see this blog post:
// https://www.uninformativ.de/blog/postings/2020-02-24/0/POSTING-en.html

// Based on the following, but modified for lariza and personal taste:
//
// easy links for surf
// christian hahn <ch radamanthys de>, sep 2010
// http://surf.suckless.org/files/easy_links/
//
// link hints for surf
// based on chromium plugin code, adapted by Nibble<.gs@gmail.com>
// http://surf.suckless.org/files/link_hints/

// Anonymous function to get private namespace.
(function() {

var charset = "sdfghjklertzuivbn".split("");
var key_follow = "f";
var key_follow_new_win = "F";

function update_highlights_or_abort()
{
    var submatch;
    var col_sel, col_unsel;
    var longest_id = 0;

    if (document.lariza_hints.state === "follow_new")
    {
        col_unsel = "#DAFFAD";
        col_sel = "#FF5D00";
    }
    else
    {
        col_unsel = "#A7FFF5";
        col_sel = "#33FF00";
    }

    for (var id in document.lariza_hints.labels)
    {
        var label = document.lariza_hints.labels[id];
        var bgcol = col_unsel;

        longest_id = Math.max(longest_id, id.length);

        if (document.lariza_hints.box.value !== "")
        {
            submatch = id.match("^" + document.lariza_hints.box.value);
            if (submatch !== null)
            {
                var href_suffix = "";
                var box_shadow_inner = "#B00000";
                if (id === document.lariza_hints.box.value)
                {
                    bgcol = col_sel;
                    box_shadow_inner = "red";
                    if (label.elem.tagName.toLowerCase() === "a")
                        href_suffix = ": <span style='font-size: 75%'>" +
                                      label.elem.href + "</span>";
                }

                var len = submatch[0].length;
                label.span.innerHTML = "<b>" + submatch[0] + "</b>" +
                                       id.substring(len, id.length) +
                                       href_suffix;
                label.span.style.visibility = "visible";

                save_parent_style(label);
                label.elem.style.boxShadow = "0 0 5pt 2pt black, 0 0 0 2pt " +
                                             box_shadow_inner + " inset";
            }
            else
            {
                label.span.style.visibility = "hidden";
                reset_parent_style(label);
            }
        }
        else
        {
            label.span.style.visibility = "visible";
            label.span.innerHTML = id;
            reset_parent_style(label);
        }
        label.span.style.backgroundColor = bgcol;
    }

    if (document.lariza_hints.box.value.length > longest_id)
        set_state("inactive");
}

function open_match()
{
    var choice = document.lariza_hints.box.value;
    var was_state = document.lariza_hints.state;

    var elem = document.lariza_hints.labels[choice].elem;
    set_state("inactive");  /* Nukes labels. */

    if (elem)
    {
        var tag_name = elem.tagName.toLowerCase();
        var type = elem.type ? elem.type.toLowerCase() : "";

        console.log("[hints] Selected elem [" + elem + "] [" + tag_name +
                    "] [" + type + "]");

        if (was_state === "follow_new" && tag_name === "a")
            window.open(elem.href);
        else if (
            (
                tag_name === "input" &&
                type !== "button" &&
                type !== "color" &&
                type !== "checkbox" &&
                type !== "file" &&
                type !== "radio" &&
                type !== "reset" &&
                type !== "submit"
            ) ||
            tag_name === "textarea" ||
            tag_name === "select"
        )
            elem.focus();
        else
            elem.click();
    }
}

function reset_parent_style(label)
{
    if (label.parent_style !== null)
        label.elem.style.boxShadow = label.parent_style.boxShadow;
}

function save_parent_style(label)
{
    if (label.parent_style === null)
    {
        var style = window.getComputedStyle(label.elem);
        label.parent_style = new Object();
        label.parent_style.boxShadow = style.getPropertyValue("boxShadow");
    }
}

function set_state(new_state)
{
    console.log("[hints] New state: " + new_state);

    document.lariza_hints.state = new_state;

    if (document.lariza_hints.state === "inactive")
    {
        nuke_labels();

        // Removing our box causes unwanted scrolling. Just hide it.
        document.lariza_hints.box.blur();
        document.lariza_hints.box.value = "";
        document.lariza_hints.box.style.visibility = "hidden";
    }
    else
    {
        if (document.lariza_hints.labels === null)
            create_labels();

        // What a terrible hack.
        //
        // Web sites often grab key events. That interferes with our
        // script. But of course, they tend to ignore key events if an
        // input element is currently focused. So ... yup, we install an
        // invisible text box (opacity 0) and focus it while follow mode
        // is active.
        var box = document.lariza_hints.box;
        if (box === null)
        {
            document.lariza_hints.box = document.createElement("input");
            box = document.lariza_hints.box;

            box.addEventListener("keydown", on_box_key);
            box.addEventListener("input", on_box_input);
            box.style.opacity = "0";
            box.style.position = "fixed";
            box.style.left = "0px";
            box.style.top = "0px";
            box.type = "text";

            box.setAttribute("lariza_input_box", "yes");

            document.body.appendChild(box);
        }

        box.style.visibility = "visible";
        box.focus();

        update_highlights_or_abort();
    }
}

function create_labels()
{
    document.lariza_hints.labels = new Object();

    var selector = "a[href]:not([href=''])";
    if (document.lariza_hints.state !== "follow_new")
    {
        selector += ", input:not([type=hidden]):not([lariza_input_box=yes])";
        selector += ", textarea, select, button";
    }

    var elements = document.body.querySelectorAll(selector);

    for (var i = 0; i < elements.length; i++)
    {
        var elem = elements[i];

        var label_id = "";
        var n = i;
        do
        {
            // Appending the next "digit" (instead of prepending it as
            // you would do it in a base conversion) scatters the labels
            // better.
            label_id += charset[n % charset.length];
            n = Math.floor(n / charset.length);
        } while (n !== 0);

        var span = document.createElement("span");
        span.style.border = "black 1pt solid";
        span.style.color = "black";
        span.style.fontFamily = "monospace";
        span.style.fontSize = "10pt";
        span.style.fontWeight = "normal";
        span.style.margin = "0px 2pt";
        span.style.position = "absolute";
        span.style.textTransform = "lowercase";
        span.style.visibility = "hidden";
        span.style.zIndex = "2147483647";  // Max for WebKit according to luakit

        document.lariza_hints.labels[label_id] = {
            "elem": elem,
            "span": span,
            "parent_style": null,
        };

        // Appending the spans as children to anchors gives better
        // placement results, but we can *only* do this for <a> ...
        var tag_name = elem.tagName.toLowerCase();
        if (tag_name === "a")
        {
            span.style.borderTopLeftRadius = "10pt";
            span.style.borderBottomLeftRadius = "10pt";
            span.style.padding = "0px 2pt 0px 5pt";
            elem.appendChild(span);
        }
        else
        {
            span.style.borderRadius = "10pt";
            span.style.padding = "0px 5pt";
            elem.parentNode.insertBefore(span, elem);
        }

        console.log("[hints] Label ID " + label_id + ", " + i +
                    " for elem [" + elem + "]");
    }
}

function nuke_labels()
{
    for (var id in document.lariza_hints.labels)
    {
        var label = document.lariza_hints.labels[id];

        reset_parent_style(label);

        var tag_name = label.elem.tagName.toLowerCase();
        if (tag_name === "a")
            label.elem.removeChild(label.span);
        else
            label.elem.parentNode.removeChild(label.span);
    }

    document.lariza_hints.labels = null;
}

function on_box_input(e)
{
    update_highlights_or_abort();
}

function on_box_key(e)
{
    if (e.key === "Escape")
    {
        e.preventDefault();
        e.stopPropagation();
        set_state("inactive");
    }
    else if (e.key === "Enter")
    {
        e.preventDefault();
        e.stopPropagation();
        open_match();
    }
}

function on_window_key(e)
{
    if (e.target.nodeName.toLowerCase() === "textarea" ||
        e.target.nodeName.toLowerCase() === "input" ||
        document.designMode === "on" ||
        e.target.contentEditable === "true")
    {
        return;
    }

    if (document.lariza_hints.state === "inactive")
    {
        if (e.key === key_follow)
            set_state("follow");
        else if (e.key === key_follow_new_win)
            set_state("follow_new");
    }
}

if (document.lariza_hints === undefined)
{
    document.lariza_hints = new Object();
    document.lariza_hints.box = null;
    document.lariza_hints.labels = null;
    document.lariza_hints.state = "inactive";

    document.addEventListener("keyup", on_window_key);

    console.log("[hints] Initialized.");
}
else
    console.log("[hints] ALREADY INSTALLED");

}());
