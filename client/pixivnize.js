// ==UserScript==
// @name        pixivnize
// @namespace   pxv
// @include     *
// @exclude     http://www.pixiv.net/search.php?*
// @version     1
// @grant       GM_xmlhttpRequest
// ==/UserScript==


var XPATH = '/html/body//text()[string-length(normalize-space(self::text())) > 0 and not(ancestor::a or ancestor::button or ancestor::iframe or ancestor::textarea or ancestor::script or ancestor::style or ancestor::x:a or ancestor::x:button or ancestor::x:iframe or ancestor::x:textarea or ancestor::x:script or ancestor::x:style)]';
var NSResolver = function () {
    return 'http://www.w3.org/1999/xhtml'
};
var expr = document.createExpression(XPATH, NSResolver);
setTimeout(function () {
    var textNodes = expr.evaluate(document, XPathResult.ORDERED_NODE_SNAPSHOT_TYPE, null);
    var range = document.createRange();
    range.selectNodeContents(document.body);
    var i = 0, tn, len;
    
    while (tn = textNodes.snapshotItem(i++)) {
        if (tn.nodeValue.length <= 1) continue;
        GM_xmlhttpRequest({
            method: "POST",
            url: "http://localhost:8000/",
            data: tn.nodeValue,
            textNode: tn,
            headers: {"Content-Type": "text"},
            onload: function(resp) {
                // console.log(this.data);
                var results = resp.responseText.split('\n');
                var html = "";
                var cur = 0;
                for (var i = 0; i < results.length; ++i) {
                    var item = results[i].split(',');
                    if (item.length === 1) continue;
                    var pos = parseInt(item[0]);
                    var len = parseInt(item[1]);
                    if (len === 1) continue;
                    html += this.data.substring(cur, pos);
                    var label = this.data.substr(pos, len);
                    var uri = 'http://www.pixiv.net/search.php?s_mode=s_tag_full&order=popular_d&manga=0&word=' + encodeURIComponent(label);
                    html += '<a href="' + uri + '" style="text-decoration:none;" target="_blank" class="pixivnize">' + label + '</a>'; //color:inherit;する？
                    cur = pos + len;
                }
                html += this.data.substr(cur);
                // console.log(html);
                var df = range.createContextualFragment(html);
                this.textNode.parentNode.replaceChild(df, this.textNode);
            }
        });
    }

    document.addEventListener('mouseover', function(ev) {
        if (/pixivnize/.test(ev.target.className)) {
            GM_xmlhttpRequest({
                method: "GET",
                url: ev.target.href,
                onload: function(xhr) {
                    var elem = document.querySelector('#pixivnize-image-container');
                    if (elem) {
                        document.body.removeChild(elem);
                    }
                    var m;
                    if (m = /<img src="(http:\/\/[^"]+)" alt="" class="_thumbnail/.exec(xhr.responseText)) {
                        // console.log(m[1]);
                        document.body.insertAdjacentHTML('beforeend', '<div id="pixivnize-image-container" style="position:fixed;right:10px;bottom:24px;max-width:200px;"><img src="' + m[1] + '" alt=""></div>');
                    }
                }
            });
        }
    }, true);
}, 500);
