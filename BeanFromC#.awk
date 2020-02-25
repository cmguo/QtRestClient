#/bin/awk -f

BEGIN {
    text = ""
    classes = ""
    primitives["string"] = "QString"
    primitives["List"] = "QList"
    primitives["Uri"] = "QString"
    print "#include <qrestbean.h>\n"
}

/public partial class / {
    class = substr($4,1,length($4)-1)
    text = "class " class " : public QRestBean<" class ">\n"
    text = text "{\n"
    text = text "    Q_GADGET\n"
    depend = ""
}

/    public / {
    sub(".",substr(tolower($3),1,1),$3)
    if (match($2, /.+\<.+\>/)) {
        i = index($2, "<");
        stun = substr($2, 1, i - 1)
        args = substr($2, i + 1, length($2) - i - 1)
        split(args, args2, ",")
        for (a in args2) {
            arg = args2[a];
            if (primitives[arg]) {
                sub(arg, primitives[arg], args)
            } else {
                depend = depend " " arg
                if (!match(templates[arg], stun))
                    templates[arg] = templates[arg] " " stun
            }
        }
        if (primitives[stun])
            sub(stun, primitives[stun], $2)
    } else {
        if (primitives[$2])
            $2 = primitives[$2]
        else
            depend = depend " " $2
    }
    props[$3] = $2
}

/^}/ {
    text = text "public:\n"
    for (p in props) {
        text = text "    Q_PROPERTY(" props[p] " " p " MEMBER " p ")\n"
    }
    text = text "public:\n"
    for (p in props) {
        text = text "    " props[p] " " p ";\n"
    }
    delete props
    text = text "};\n\n"
    text = text "Q_DECLARE_METATYPE(" class ")\n"
    texts[class] = text;
    depends[class] = depend
    classes = classes  " " class
}

function print_class(c, d, depends2, templates2)
{
    if (texts[c]) {
        #if (!depends[c])
        #    print "no dep " c;
        #print c "->" depends[c]
        split(depends[c], depends2, " ");
        delete depends[c]
        for (d in depends2) {
            #print d ": " depends2[d]
            print_class(depends2[d])
        }
        print texts[c]
        delete texts[c]
        split(templates[c], templates2, " ");
        for (d in templates2) {
            template = template  "            " c "::register" templates2[d] "();\n"
        }
        print
        delete templates[c]
    } else {
        #print "no class " c
    }
}

END {
    template = "namespace {\n"
    template = template "    struct initializer {\n"
    template = template "        initializer() {\n"
    split(classes, classes2, " ");
    for (c in classes2) {
        print_class(classes2[c])
    }
    template = template "        }\n"
    template = template "    } dummy;\n"
    template = template "}\n"
    print template
}