#正则表达式中参数g、i、m的作用（share）

 
##(1)  参数 g
 
g 只影响于 exec、match 方法。

若不指定 g，则：每次调用 exec 都只返回第一个匹配；match 也是只返回第一个匹配。

若指定 g，则：每次调用 exec 都从上一个匹配之后查找新的匹配；match 则是返回所有的匹配。

 

还有一种情况，就是使用 string 对象的 replace 方法时，指定 g 表示替换所有。

var str = “1a1b1c”;

alert(str.replace(/1/, “”)); // a1b1c

alert(str.replace(/1/g, “”)); // abc

 
##(2) 参数 i

参数 i 是指忽略大小写，注意仅是忽略大小写，并不忽略全半角。

 
##(3) 参数 m

m 影响对行首、行尾的解释，也就是影响 ^、$。

若不指定 m，则：^ 只在字符串的最开头，$ 只在字符串的最结尾。

若指定 m，则：^ 在字符串每一行的开头，$ 在字符串第一行的结尾。

var str = “123\r\n456″;

var reg1 = new RegExp(“^123$\r\n^456$”, “m”); // true

var reg2 = new RegExp(“^123$\r\n^456$”, “”); // false

var reg3 = new RegExp(“123\r\n456″, “m”); // true

var reg4 = new RegExp(“123\r\n456″, “”); // true

alert(reg1.test(str) + “\r\n” + reg2.test(str) + “\r\n” + reg3.test(str) + “\r\n” + reg4.test(str));