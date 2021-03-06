<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet
    version='1.0'
    xmlns:xsl='http://www.w3.org/1999/XSL/Transform'
>

<xsl:template match='pairCuts'>
    <xsl:call-template name='pairCutHeader'/>
    <xsl:apply-templates/>
</xsl:template>

<!-- Note that I add a peroiod<cr> after Comment.
     Without some text the <cr> gets stripped. -->
<xsl:template match='pairCut'>
    <xsl:apply-templates/>
    <xsl:for-each select='cutName'>
        <xsl:value-of select="."/>
    </xsl:for-each>
    <xsl:for-each select='Value'>
        <xsl:text>,</xsl:text>
        <xsl:value-of select="."/>
    </xsl:for-each>
    <xsl:text>     #</xsl:text>
    <xsl:value-of select='@Comment'/>
    <xsl:text>.
</xsl:text>
</xsl:template>

<xsl:template name='pairCutHeader'>
# ********************************************
# *************** Pair Cuts ******************
# *** format = variable,value1,value1  *******
# ********************************************
</xsl:template>

<!-- Want to ignore most nodes. -->
<xsl:template match='text()'>
</xsl:template>

</xsl:stylesheet>
