CFLAGS = -Os
RM := rm -rf
CP := cp
redirect_rewrite: redirect_rewrite.c urldecode.c
	$(CC) -o $@ $(CFLAGS) redirect_rewrite.c urldecode.c -I.
clean:
	-$(RM) redirect_rewrite
	-@echo 'Cleanup complete'
install:
	-$(CP) redirect_rewrite /usr/local/bin/redirect_rewrite
	-@echo ''
