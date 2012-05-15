#
# "Software pw3270, desenvolvido com base nos códigos fontes do WC3270  e X3270
# (Paul Mattes Paul.Mattes@usa.net), de emulação de terminal 3270 para acesso a
# aplicativos mainframe. Registro no INPI sob o nome G3270.
#
# Copyright (C) <2008> <Banco do Brasil S.A.>
#
# Este programa é software livre. Você pode redistribuí-lo e/ou modificá-lo sob
# os termos da GPL v.2 - Licença Pública Geral  GNU,  conforme  publicado  pela
# Free Software Foundation.
#
# Este programa é distribuído na expectativa de  ser  útil,  mas  SEM  QUALQUER
# GARANTIA; sem mesmo a garantia implícita de COMERCIALIZAÇÃO ou  de  ADEQUAÇÃO
# A QUALQUER PROPÓSITO EM PARTICULAR. Consulte a Licença Pública Geral GNU para
# obter mais detalhes.
#
# Você deve ter recebido uma cópia da Licença Pública Geral GNU junto com este
# programa;  se  não, escreva para a Free Software Foundation, Inc., 59 Temple
# Place, Suite 330, Boston, MA, 02111-1307, USA
#
# Contatos:
#
# perry.werneck@gmail.com	(Alexandre Perry de Souza Werneck)
#

JAVAH=javah
NDKBUILD=/opt/android-ndk-r8/ndk-build
CLASSPATH=br/com/bb/pw3270

MKDIR=/bin/mkdir -p
CONVERT=rsvg-convert

RESOLUTION=xhdpi mdpi ldpi hdpi

Debug: jni/lib3270jni.h $(foreach DIR, $(RESOLUTION), res/drawable-$(DIR)/ic_launcher.png)
	@$(NDKBUILD)

clean:
	@rm -fr obj
	@rm -f jni/lib3270jni.h
	@rm -f $(foreach DIR, $(RESOLUTION), res/drawable-$(DIR)/ic_launcher.png)

res/drawable-xhdpi/ic_launcher.png: ../src/pw3270/pixmaps/pw3270.svg
	@echo "  GEN  `basename $@`"
	@$(MKDIR) `dirname $@`
	@$(CONVERT) --format=png --width=96 --height=96 --output=$@ $<

res/drawable-mdpi/ic_launcher.png: ../src/pw3270/pixmaps/pw3270.svg
	@echo "  GEN  `basename $@`"
	@$(MKDIR) `dirname $@`
	@$(CONVERT) --format=png --width=48 --height=48 --output=$@ $<

res/drawable-ldpi/ic_launcher.png: ../src/pw3270/pixmaps/pw3270.svg
	@echo "  GEN  `basename $@`"
	@$(MKDIR) `dirname $@`
	@$(CONVERT) --format=png --width=36 --height=36 --output=$@ $<

res/drawable-hdpi/ic_launcher.png: ../src/pw3270/pixmaps/pw3270.svg
	@echo "  GEN  `basename $@`"
	@$(MKDIR) `dirname $@`
	@$(CONVERT) --format=png --width=72 --height=72 --output=$@ $<

jni/lib3270jni.h: bin/classes/$(CLASSPATH)/lib3270.class
	@echo "  GEN  `basename $@`"
	@$(JAVAH) -o $@ -classpath bin/classes $(subst /,.,$(CLASSPATH)).lib3270
