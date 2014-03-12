// ==UserScript==
// @name        pixivnize
// @namespace   pxv
// @include     *
// @version     1
// @grant       GM_xmlhttpRequest
// ==/UserScript==


var XPATH = '/html/body/descendant::text()[string-length(normalize-space(self::text())) > 0 and not(ancestor::a or ancestor::iframe or ancestor::textarea or ancestor::script or ancestor::style or ancestor::x:a or ancestor::x:iframe or ancestor::x:textarea or ancestor::x:script or ancestor::x:style)]';
var NSResolver = function () {
    return 'http://www.w3.org/1999/xhtml'
};
var expr = document.createExpression(XPATH, NSResolver);
(function () {
    var textNodes = expr.evaluate(document, XPathResult.ORDERED_NODE_SNAPSHOT_TYPE, null);
    var range = document.createRange();
    range.selectNodeContents(document.body);
    var i = 0, tn, len;
    console.log('pixivnize');
    while (tn = textNodes.snapshotItem(i++)) {
        console.log(tn.nodeValue);
        if (tn.nodeValue.length <= 1) continue;
        GM_xmlhttpRequest({
            method: "POST",
            url: "http://localhost:8000/",
            data: tn.nodeValue,
            headers: {"Content-Type": "text/plain"},
            onload: function(response) {
                console.log(response.responseText);
            }
        });
    }
})();