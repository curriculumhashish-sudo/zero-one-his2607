package com.zeroone.star.statdata.filter;

import jakarta.servlet.Filter;
import jakarta.servlet.FilterChain;
import jakarta.servlet.ServletException;
import jakarta.servlet.ServletRequest;
import jakarta.servlet.ServletResponse;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletRequestWrapper;
import org.springframework.util.StringUtils;

import java.io.IOException;
import java.net.URLDecoder;
import java.nio.charset.StandardCharsets;
import java.util.Collections;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;

public class UserHeaderDecodeFilter implements Filter {

    private static final String USER_HEADER = "user";

    @Override
    public void doFilter(ServletRequest request,
                         ServletResponse response,
                         FilterChain chain) throws IOException, ServletException {
        HttpServletRequest httpRequest = (HttpServletRequest) request;
        String rawUser = httpRequest.getHeader(USER_HEADER);
        if (StringUtils.hasText(rawUser)) {
            String decoded;
            try {
                decoded = URLDecoder.decode(rawUser, StandardCharsets.UTF_8);
            } catch (IllegalArgumentException e) {
                decoded = rawUser;
            }
            chain.doFilter(new UserHeaderOverrideRequest(httpRequest, decoded), response);
        } else {
            chain.doFilter(request, response);
        }
    }

    private static class UserHeaderOverrideRequest extends HttpServletRequestWrapper {
        private final String decodedUser;

        UserHeaderOverrideRequest(HttpServletRequest request, String decodedUser) {
            super(request);
            this.decodedUser = decodedUser;
        }

        @Override
        public String getHeader(String name) {
            if (USER_HEADER.equalsIgnoreCase(name)) {
                return decodedUser;
            }
            return super.getHeader(name);
        }

        @Override
        public Enumeration<String> getHeaders(String name) {
            if (USER_HEADER.equalsIgnoreCase(name)) {
                return Collections.enumeration(Collections.singletonList(decodedUser));
            }
            return super.getHeaders(name);
        }

        @Override
        public Enumeration<String> getHeaderNames() {
            Enumeration<String> names = super.getHeaderNames();
            Map<String, Boolean> seen = new HashMap<>();
            while (names.hasMoreElements()) {
                seen.put(names.nextElement().toLowerCase(), Boolean.TRUE);
            }
            if (!seen.containsKey(USER_HEADER)) {
                java.util.List<String> all = new java.util.ArrayList<>();
                all.addAll(Collections.list(super.getHeaderNames()));
                all.add(USER_HEADER);
                return Collections.enumeration(all);
            }
            return super.getHeaderNames();
        }
    }
}