# That script compiles the worms application for the target
# platform, if the BR/output/host/environment-setup has been
# sourced to the current shell session.
OBJS := SRP.o

SRP: $(OBJS)
	$(CC) -o SRP $(CFLAGS) $(LDFLAGS) $(OBJS) -lperiphery
	
$(OBJS) : %.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

