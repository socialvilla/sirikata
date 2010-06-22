#!/usr/bin/python

import subprocess
import random

color_seed=0
# Get a random color for a graph
def get_random_color():
    is_not_804 = subprocess.call(['grep', '-q', '8[.]04', '/etc/lsb-release'])

    if not is_not_804:
        set_colors = [
            'aliceblue',
            'antiquewhite',
            'aqua',
            'aquamarine',
            'azure',
            'beige',
            'bisque',
            'black',
            'blanchedalmond',
            'blue',
            'blueviolet',
            'brown',
            'burlywood',
            'cadetblue',
            'chartreuse',
            'chocolate',
            'coral',
            'cornflowerblue',
            'cornsilk',
            'crimson',
            'cyan',
            'darkblue',
            'darkcyan',
            'darkgoldenrod',
            'darkgray',
            'darkgreen',
            'darkkhaki',
            'darkmagenta',
            'darkolivegreen',
            'darkorange',
            'darkorchid',
            'darkred',
            'darksalmon',
            'darkseagreen',
            'darkslateblue',
            'darkslategray',
            'darkturquoise',
            'darkviolet',
            'deeppink',
            'deepskyblue',
            'dimgray',
            'dodgerblue',
            'firebrick',
            'floralwhite',
            'forestgreen',
            'fuchsia',
            'gainsboro',
            'ghostwhite',
            'gold',
            'goldenrod',
            'gray',
            'green',
            'greenyellow',
            'honeydew',
            'hotpink',
            'indianred',
            'indigo',
            'ivory',
            'khaki',
            'lavender',
            'lavenderblush',
            'lawngreen',
            'lemonchiffon',
            'lightblue',
            'lightcoral',
            'lightcyan',
            'lightgoldenrodyellow',
            'lightgreen',
            'lightgrey',
            'lightpink',
            'lightsalmon',
            'lightseagreen',
            'lightskyblue',
            'lightslategray',
            'lightsteelblue',
            'lightyellow',
            'lime',
            'limegreen',
            'linen',
            'magenta',
            'maroon',
            'mediumaquamarine',
            'mediumblue',
            'mediumorchid',
            'mediumpurple',
            'mediumseagreen',
            'mediumslateblue',
            'mediumspringgreen',
            'mediumturquoise',
            'mediumvioletred',
            'midnightblue',
            'mintcream',
            'mistyrose',
            'moccasin',
            'navajowhite',
            'navy',
            'oldlace',
            'olive',
            'olivedrab',
            'orange',
            'orangered',
            'orchid',
            'palegoldenrod',
            'palegreen',
            'palevioletred',
            'papayawhip',
            'peachpuff',
            'peru',
            'pink',
            'plum',
            'powderblue',
            'purple',
            'red',
            'rosybrown',
            'royalblue',
            'saddlebrown',
            'salmon',
            'sandybrown',
            'seagreen',
            'seashell',
            'sienna',
            'silver',
            'skyblue',
            'slateblue',
            'slategray',
            'snow',
            'springgreen',
            'steelblue',
            'tan',
            'teal',
            'thistle',
            'tomato',
            'turquoise',
            'violet',
            'wheat',
            'white',
            'whitesmoke',
            'yellow',
            'yellowgreen'
            ]
        return set_colors[ int(random.uniform(0, len(set_colors))) ]

    global color_seed;
    color_list=[(51/255.,102/255.,204/255.),
              (153/255.,204/255.,255/255.),
              (153/255.,153/255.,51/255.),
              (102/255.,102/255.,153/255.),
              (204/255.,153/255.,51/255.),
              (0/255.,102/255.,102/255.),
              (51/255.,153/255.,255/255.),
              (153/255.,51/255.,0/255.),
              (204/255.,204/255.,153/255.),
              (102/255.,102/255.,102/255.),
              (255/255.,204/255.,102/255.),
              (102/255.,153/255.,204/255.),
              (102/255.,51/255.,102/255.),
              (153/255.,153/255.,204/255.),
              (204/255.,204/255.,204/255.),
              (102/255.,153/255.,153/255.),
              (204/255.,204/255.,102/255.),
              (204/255.,102/255.,0/255.),
              (153/255.,153/255.,255/255.),
              (0/255.,102/255.,204/255.),
              (153/255.,204/255.,204/255.),
              (153/255.,153/255.,153/255.),
              (255/255.,204/255.,0/255.),
              (0/255.,153/255.,153/255.),
              (153/255.,204/255.,51/255.),
              (255/255.,153/255.,0/255.),
              (153/255.,153/255.,102/255.),
              (102/255.,204/255.,204/255.),
              (51/255.,153/255.,102/255.),
              (204/255.,204/255.,51/255.)]
    retval=color_list[color_seed%len(color_list)];
    divisor=int(color_seed/len(color_list))+1;
    retval=(retval[0]/divisor,
            retval[1]/divisor,
            retval[2]/divisor)
    color_seed+=1;
    return retval;