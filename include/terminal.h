class PTY {
public:
	std::string slave;

	PTY();
	bool attachMaster();
	bool attachSlave();
	~PTY();

private:
	int masterFd{-1}, slaveFd{-1};
};