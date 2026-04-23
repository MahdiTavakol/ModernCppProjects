#pragma once


class settings
{
public:
	settings() = default;
	settings(int mode_) : mode{ mode_ } {}
	int return_mode() { return mode; }
	virtual void set_mode(int mode_) { mode = mode_; }
	virtual void check_validity() const = 0;

protected:
	int mode = 0;
};