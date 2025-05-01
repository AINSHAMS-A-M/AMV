#pragma once

/// Simple interface for pages that can refresh their content
/// A Base class that all the pages will inherit, it will allow to get up-to-date data
class Refreshable {
public:
    virtual ~Refreshable() = default;
    virtual void refresh() = 0;
};
