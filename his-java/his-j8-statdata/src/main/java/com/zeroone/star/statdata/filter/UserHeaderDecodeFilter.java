package com.zeroone.star.statdata.filter;

import jakarta.servlet.FilterChain;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletRequestWrapper;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.core.Ordered;
import org.springframework.core.annotation.Order;
import org.springframework.stereotype.Component;
import org.springframework.util.StringUtils;
import org.springframework.web.filter.OncePerRequestFilter;

import java.io.IOException;
import java.net.URLDecoder;
import java.nio.charset.StandardCharsets;
import java.util.Collections;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;

@Component
@Order(Ordered.HIGHEST_PRECEDENCE)
public class UserHeaderDecodeFilter extends OncePerRequestFilter {

    private static final String USER_HEADER = "user";

    @Override
    protected void doFilterInternal(HttpServletRequest request,
                                    HttpServletResponse response,
                                    FilterChain filterChain) throws ServletException, IOException {
        String rawUser = request.getHeader(USER_HEADER);
        if (StringUtils.hasText(rawUser)) {
            String decoded;
            try {
                decoded = URLDecoder.decode(rawUser, StandardCharsets.UTF_8);
            } catch (IllegalArgumentException e) {
                decoded = rawUser;
            }
            filterChain.doFilter(new UserHeaderOverrideRequest(request, decoded), response);
        } else {
            filterChain.doFilter(request, response);
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